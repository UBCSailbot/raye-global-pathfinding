// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherHexMap.h"
#include <logic/StandardCalc.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include "grib/gribParse.h"
#include <eccodes.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


WeatherHexMap::WeatherHexMap(const HexPlanet &planet, const uint32_t time_steps,
                             int start_lat, int start_lon, int end_lat, int end_lon,
                             bool generate_new_grib, const std::string & file_name)
    : planet_(planet), steps_(time_steps) {
  weather_data_.resize(boost::extents[planet_.vertex_count()][time_steps]);
  // std::cout << "TIME STEPS " << time_steps << std::endl;
  // std::cout << "TIME STEPS " << steps_ << std::endl;

  int gribIndex, north = start_lat, south = end_lat, east = start_lon, west = end_lon;
  int lat, lon;

  if (generate_new_grib) {
    std::string url = UrlBuilder::BuildURL(std::to_string(north), std::to_string(south),
                                           std::to_string(east), std::to_string(west));
    UrlDownloader::Downloader(url);
  }

  gribParse file = gribParse(file_name);
  file.saveKML();

  for (WeatherMatrix::index i = 0; i < planet_.vertex_count(); ++i) {
    for (WeatherMatrix::index j = 0; j < steps_; j++) {
      // std::cout << "START " << std::endl;
      if (j >= 10) {
        // std::cout << "HERE" << std::endl;
      }
      boost::array<WeatherMatrix::index, 2> ind = {{i, j}};
      const auto &coord = planet.vertex(i).coordinate;
      lat = coord.round_to_int_latitude();
      lon = coord.round_to_int_longitude();

      lon = lon < 0 ? lon+360 : lon;  // convert negative longitudes to positive

      // std::cout << "i  " << i << std::endl;
      // std::cout << "j  " << j << std::endl;
      // std::cout << "steps_  " << steps_ << std::endl;
      // std::cout << "planet_.vertex_count()  " << planet_.vertex_count() << std::endl;
      // std::cout << "weather_data_.size() " << weather_data_.size() << std::endl;
      if (lat > north || lat < south || lon > east || lon < west) {
        // std::cout << "In if " << std::endl;
        weather_data_(ind) = WeatherDatum{0.0, 0.0, 0.0, 0.0, 0.0};
        // std::cout << "In if 2" << std::endl;
        continue;
      }

      // std::cout << "Out if" << std::endl;
      gribIndex = (lat-south) * (east-west+1) + (lon-west);
      // std::cout << "Out if" << std::endl;
      if (gribIndex >= file.number_of_points_) gribIndex = file.number_of_points_-1;
      // std::cout << "Out if" << std::endl;
      // std::cout << "file.magnitudes[j][gribIndex] " << j << " " << gribIndex << std::endl;
      // std::cout << "file.magnitudes.size() " << file.magnitudes.size() << std::endl;
      // std::cout << "file.magnitudes[j].size() " << file.magnitudes[j].size() << std::endl;
      weather_data_(ind) = WeatherDatum{file.magnitudes[j][gribIndex], file.angles[j][gribIndex], 0.0, 0.0, 0.0};
      // std::cout << "START " << std::endl;
    }
  }
}

const WeatherDatum& WeatherHexMap::get_weather(HexVertexId vertex_id,
                                             uint32_t time) {
  if (vertex_id >= planet_.vertex_count() || vertex_id < 0 || time < 0) {
    std::cout << "vertex_id " << vertex_id << std::endl;
    std::cout << "time " << time << std::endl;
    std::cout << "planet_.vertex_count() " << planet_.vertex_count() << std::endl;
    std::cout << "steps_ " << steps_ << std::endl;
    throw std::runtime_error("Invalid vertex ID or time.");
  }

  if (time >= steps_) {
    time = steps_ - 1;
  }

  boost::array<WeatherMatrix::index, 2> ind = {{vertex_id, time}};
  return weather_data_(ind);
}
