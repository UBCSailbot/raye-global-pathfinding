// Copyright 2017 UBC Sailbot

#include "pathfinding/PathfinderResultPrinter.h"

#include <iomanip>
#include <fstream>
#include <pathfinding/WeatherCostCalculator.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include <string>
#include "eccodes.h"
#include "grib/gribParse.h"


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

std::string PathfinderResultPrinter::PrintKML(HexPlanet &planet, const Pathfinder::Result &result) {
  std::ofstream handle;
  std::stringstream ss;
  int gribIndex,north=48, south=20, east=237, west=204;
  int lat,lon;
  std::string file_name = "data.grb";

  std::string url = UrlBuilder::BuildURL(std::to_string(north), std::to_string(south), std::to_string(east), std::to_string(west));
  UrlDownloader::Downloader(url);

  gribParse file = gribParse(file_name);
  int count = 0, sum = 0;

/*  WeatherHexMap disp_weather_map = WeatherHexMap(planet, 3);
  auto disp_wmap_pointer = std::make_unique<WeatherHexMap>(disp_weather_map);
*/
  // Throw an exception on failure to open the file or on a write error.
  handle.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  handle.open("Path.kml");

  handle << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    handle << coord.to_string_longitude() << ", " << coord.to_string_latitude() << std::endl;
    ss << coord.to_string_longitude() << ", " << coord.to_string_latitude() << std::endl;

    lat = coord.round_to_int_latitude();
    lon = coord.round_to_int_longitude();
    lon = lon < 0 ? lon+360 : lon;
    gribIndex = (lat-south) * (east-west+1) + (lon-west);
    sum += file.magnitudes[gribIndex];
    count++;

/*    sum += disp_wmap_pointer->get_weather(id, 0).wind_speed;
    count++;*/
  }

  handle << "</coordinates></LineString></Placemark></Document></kml>\n";

  ss << "</coordinates></LineString></Placemark></Document></kml>\n" << std::endl;

  ss << "Avg wind speed is: " << std::to_string(sum/count) << std::endl;

  handle.close();

  return ss.str();
}
