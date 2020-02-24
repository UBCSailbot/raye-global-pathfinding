// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherHexMap.h"
#include <logic/StandardCalc.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include "eccodes.h"
#include "grib/gribParse.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

WeatherHexMap::WeatherHexMap(const HexPlanet &planet, const uint32_t time_steps, int start_lat, int start_lon, int end_lat, int end_lon)
    : planet_(planet), steps_(time_steps) {
  weather_data_.resize(boost::extents[planet_.vertex_count()][time_steps]);

  int gribIndex,north=start_lat, south=end_lat, east=start_lon, west=end_lon;
  int lat,lon;
  std::string file_name = "data.grb";

  std::string url = UrlBuilder::BuildURL(std::to_string(north), std::to_string(south), std::to_string(east), std::to_string(west));
  UrlDownloader::Downloader(url);

  gribParse file = gribParse(file_name);
  file.saveKML();

  for (WeatherMatrix::index i = 0; i < steps_; ++i) {
    for (WeatherMatrix::index j = 0; j < planet_.vertex_count(); j++) {
      boost::array<WeatherMatrix::index, 2> ind = {{i, j}};
      const auto &coord = planet.vertex(j).coordinate;
      lat = coord.round_to_int_latitude();
      lon = coord.round_to_int_longitude();

      lon = lon < 0 ? lon+360 : lon;    //convert negative longitudes to positive

      if(lat > north || lat < south || lon > east || lon < west){
        weather_data_(ind) = WeatherDatum{0.0, 0.0, 0.0, 0.0, 0.0};
        continue;
      }

      gribIndex = (lat-south) * (east-west+1) + (lon-west);
      if (gribIndex >= file.number_of_points_) gribIndex = file.number_of_points_-1;
      weather_data_(ind) = WeatherDatum{file.magnitudes[i][gribIndex], file.angles[i][gribIndex], 0.0, 0.0, 0.0};
    }
  }
}

const WeatherDatum& WeatherHexMap::get_weather(HexVertexId vertex_id,
                                             uint32_t time) {
  if (vertex_id >= planet_.vertex_count())// || time >= steps_)
    throw std::runtime_error("Invalid vertex ID or time.");

  boost::array<WeatherMatrix::index, 2> ind = {{vertex_id, 0}};
  return weather_data_(ind);
}
