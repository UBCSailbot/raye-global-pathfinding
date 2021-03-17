// Copyright 2017 UBC Sailbot

#include "pathfinding/PathfinderResultPrinter.h"
#include "pathfinding/WeatherCostCalculator.h"
#include "pathfinding/WeatherHexMap.h"
#include "grib/gribParse.h"
#include "grib/UrlBuilder.h"
#include "grib/UrlDownloader.h"
#include "logic/StandardCalc.h"

#include <iomanip>
#include <fstream>
#include <string>
#include <eccodes.h>
#include <vector>


std::string PathfinderResultPrinter::PrintDefault(const Pathfinder::Result &result) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(1)
     << "Cost: " << result.cost << " (distance in meters)" << std::endl
     << "Time: " << result.time << std::endl;

  ss << "Path: " << std::endl;
  for (auto point : result.path) {
    ss << point << " ";
  }
  ss << std::endl;

  return ss.str();
}

std::string PathfinderResultPrinter::PrintCoordinates(HexPlanet &planet, const Pathfinder::Result &result) {
  std::stringstream ss;

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    ss << id << ", " << coord.to_string_latitude() << ", " << coord.to_string_longitude() << std::endl;
  }

  return ss.str();
}


std::vector<std::pair<double, double>> PathfinderResultPrinter::GetVector(HexPlanet &planet,
                                                                          const Pathfinder::Result &result) {
  std::vector<std::pair<double, double>> pathResult;

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    auto lat_str = coord.to_string_latitude();
    auto lon_str = coord.to_string_longitude();

    pathResult.push_back(std::make_pair(std::stod(lat_str), std::stod(lon_str)));
  }

  return pathResult;
}

std::string PathfinderResultPrinter::PrintKML(HexPlanet &planet,
                                              const Pathfinder::Result &result,
                                              int weather_factor,
                                              int pointToPrint) {
  std::ofstream handle;
  std::stringstream ss;
  int north = 49, south = 21, east = 235, west = 203;
  int countPoints = 0;
  std::string latToPrint, lonToPrint;
  std::string file_name = "data.grb";

  std::vector<std::pair<double, double>> pathResult;

  gribParse file = gribParse(file_name);

  HexVertexId old_id;

  handle.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  handle.open("Path.kml");

  handle << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  int count = 0;
  double sum = 0, max = 0, totalWeatherCost = 0;
  uint32_t totalDist = 0.0;
  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;

    auto lon_str = coord.to_string_longitude();
    auto lat_str = coord.to_string_latitude();

    pathResult.push_back(std::make_pair(std::stod(lon_str), std::stod(lat_str)));

    handle << lon_str << "," << lat_str << std::endl;
    ss << pathResult.back().first << "," << pathResult.back().second << std::endl;

    countPoints++;

    if (pointToPrint == countPoints) {
      latToPrint = lat_str;
      lonToPrint = std::stod(lon_str) < 0 ? std::to_string(360 + std::stod(lon_str)) : lon_str;
    }

    if (count > 0) {
        totalDist += planet.DistanceBetweenVertices(old_id, id);
    }

    int lat, lon;
    lat = coord.round_to_int_latitude();
    lon = coord.round_to_int_longitude();
    lon = lon < 0 ? lon+360 : lon;
    int gribIndex = (lat-south) * (east-west+1) + (lon-west);

    int time_step;
    double dist = sqrt(pow(lat-north, 2) + pow(lon-east, 2));
    if (dist < 2)
      time_step = 0;
    else if (dist < 4)
      time_step = 1;
    else if (dist < 6)
      time_step = 2;
    else
      time_step = 3;
    double current_wind = file.magnitudes[time_step][gribIndex];

    if (current_wind <= 5) {  // See https://www.desmos.com/calculator/md1byjfsl2
      totalWeatherCost += 17 - current_wind;
    } else if (current_wind < 11) {
      totalWeatherCost += 22 - current_wind * 2;
    } else if (current_wind < 16) {
      totalWeatherCost += 0;
    } else if (current_wind < 30) {
      totalWeatherCost += current_wind * 2 - 32;
    } else {
      totalWeatherCost += 1000;
    }

    sum += current_wind;
    if (current_wind > max) max = current_wind;
    count++;

    old_id = id;
  }

  handle << "</coordinates></LineString></Placemark></Document></kml>\n";

  ss << "</coordinates></LineString></Placemark></Document></kml>\n" << std::endl;

  ss << "Weather factor: " << std::to_string(weather_factor) << std::endl;

  ss << "Avg wind speed is: " << std::to_string(sum/count) << std::endl;

  ss << "Max wind speed is: " << std::to_string(max) << std::endl;

  ss << "Raw weather cost is: " << std::to_string(totalWeatherCost) << std::endl;

  ss << "Avg distance is: " << std::to_string(totalDist/(count*1000))
     << "." << std::to_string((totalDist/count) % 1000) << std::endl;

  ss << "Total distance is: " << std::to_string(totalDist/(1000))
     << "." << std::to_string((totalDist) % 1000) << std::endl;

  ss << "Total weather cost is: " << std::to_string(totalWeatherCost*weather_factor) << std::endl;

  ss << "Total distance cost is: " << std::to_string(totalDist) << std::endl;


  handle.close();

  if (pointToPrint > 0) {
    ss << std::endl << latToPrint << " " << lonToPrint << std::endl;
  }

  return ss.str();
}
