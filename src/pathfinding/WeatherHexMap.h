// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_WEATHERHEXMAP_H_
#define PATHFINDING_WEATHERHEXMAP_H_

#include "boost/multi_array.hpp"
#include "datatypes/WeatherDatum.h"
#include "planet/HexPlanet.h"

/**
 * @brief Stores weather data over time for each vertex of a HexPlanet.
 */
class WeatherHexMap {
 public:
  /**
   * Initializes a map of weather data for each vertex of the planet.
   * Each WeatherDatum contains weather info for one point in time.
   * @param planet The planet.
   * @param time_steps How many |WeatherDatum|s to store for each vertex.
   */
  explicit WeatherHexMap(const HexPlanet &planet,
                         const uint32_t time_steps,
                         int start_lat,
                         int start_lon,
                         int end_lat,
                         int end_lon);

  /**
   * Gets the |WeatherDatum| associated with a specific vertex at a specified
   * number of time steps from initialization.
   * @param vertex_id The id of the vertex.
   * @param time The time, expressed as number of time steps into the future.
   * @throw std::runtime_error |vertex_id| is invalid
   *    or |time_step| is out of range.
   * @return The |WeatherDatum| associated with that vertex at that time.
   */
  const WeatherDatum& get_weather(HexVertexId vertex_id, uint32_t time_steps);

 private:
  const HexPlanet &planet_;
  const uint32_t steps_;
  typedef boost::multi_array<WeatherDatum, 2> WeatherMatrix;
  WeatherMatrix weather_data_;
};

#endif  // PATHFINDING_WEATHERHEXMAP_H_

#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

extern int weather_factor;
extern bool generate_new_grib;

#endif
