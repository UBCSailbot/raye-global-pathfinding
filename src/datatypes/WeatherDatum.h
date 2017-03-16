// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_WEATHERDATUM_H_
#define DATATYPES_WEATHERDATUM_H_

/**
 * Contains weather information for a single vertex on the Planet.
 */
struct WeatherDatum {
  /// Wind speed in m/s
  double wind_speed;

  /// Wind direction in radians (clockwise from North)
  double wind_direction;

  /// Current speed in m/s
  double current_speed;

  /// Current direction in radians (clockwise from North)
  double current_direction;

  /// Wave height in meters
  double wave_height;
};

#endif  // DATATYPES_WEATHERDATUM_H_
