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
                             bool generate_new_grib, const std::string & file_name, bool use_csvs,
                             const std::string & output_csvs_folder, bool preserveKml)
    : planet_(planet), steps_(time_steps) {
  weather_data_.resize(boost::extents[planet_.vertex_count()][time_steps]);

  int gribIndex;
  int north = std::max(start_lat, end_lat);
  int south = std::min(start_lat, end_lat);
  int east = std::max(start_lon, end_lon);
  int west = std::min(start_lon, end_lon);
  int lat, lon;

  if (generate_new_grib) {
    std::string url = UrlBuilder::BuildURL(std::to_string(north), std::to_string(south),
                                           std::to_string(east), std::to_string(west));
    UrlDownloader::Downloader(url);
  }

  gribParse file = gribParse(file_name, time_steps, use_csvs, output_csvs_folder);
  file.saveKML(preserveKml);

  for (WeatherMatrix::index vertex_id = 0; vertex_id < (WeatherMatrix::index)planet_.vertex_count(); ++vertex_id) {
    for (WeatherMatrix::index time_step = 0; time_step < steps_; time_step++) {
      boost::array<WeatherMatrix::index, 2> ind = {{vertex_id, time_step}};
      const auto &coord = planet.vertex(vertex_id).coordinate;
      lat = coord.round_to_int_latitude();
      lon = coord.round_to_int_longitude();
      lon = lon < 0 ? lon+360 : lon;  // convert negative longitudes to positive

      if (lat > north || lat < south || lon > east || lon < west) {
        // If out of bounds, put a high wind there to avoid going there
        weather_data_(ind) = WeatherDatum{1000.0, 0.0, 0.0, 0.0, 0.0};
        continue;
      }

      gribIndex = (lat-south) * (east-west+1) + (lon-west);
      if (gribIndex >= file.number_of_points_) gribIndex = file.number_of_points_-1;
      weather_data_(ind) = WeatherDatum{file.magnitudes[time_step][gribIndex],
                                        file.angles[time_step][gribIndex], 0.0, 0.0, 0.0};
    }
  }
}

const WeatherDatum& WeatherHexMap::get_weather(HexVertexId vertex_id,
                                             uint32_t time) {
  if (vertex_id >= planet_.vertex_count()) {
    throw std::runtime_error("Invalid vertex ID.");
  }

  // Clamp time to be at max allowable value
  if (time >= steps_) {
    time = steps_ - 1;
  }

  boost::array<WeatherMatrix::index, 2> ind = {{vertex_id, time}};
  return weather_data_(ind);
}
