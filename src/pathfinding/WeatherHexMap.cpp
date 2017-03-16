// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherHexMap.h"

WeatherHexMap::WeatherHexMap(const HexPlanet &planet, const uint32_t time_steps)
    : planet_(planet), steps_(time_steps) {
  weather_data_.resize(boost::extents[planet_.vertex_count()][time_steps]);

  for (WeatherMatrix::index i = 0; i < planet_.vertex_count(); ++i) {
    for (WeatherMatrix::index j = 0; j < steps_; ++j) {
      boost::array<WeatherMatrix::index, 2> ind = {{i, j}};
      // just use WeatherDatum with all zeros for now
      weather_data_(ind) = WeatherDatum{0.0, 0.0, 0.0, 0.0, 0.0};
    }
  }
}

const WeatherDatum& WeatherHexMap::get_weather(HexVertexId vertex_id,
                                             uint32_t time) {
  if (vertex_id >= planet_.vertex_count() || time >= steps_)
    throw std::runtime_error("Invalid vertex ID or time.");

  boost::array<WeatherMatrix::index, 2> ind = {{vertex_id, time}};
  return weather_data_(ind);
}
