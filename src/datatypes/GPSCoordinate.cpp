// Copyright 2016 UBC Sailbot

#include <iomanip>
#include <sstream>
#include <cmath>

#include "datatypes/GPSCoordinate.h"

GPSCoordinate::GPSCoordinate() {}

GPSCoordinate::GPSCoordinate(int32_t latitude_exact, int32_t longitude_exact)
    : latitude_exact_(latitude_exact), longitude_exact_(longitude_exact) {}

void GPSCoordinate::set_lat_exact(int32_t latitude_exact) {
  latitude_exact_ = latitude_exact;
}

void GPSCoordinate::set_lng_exact(int32_t longitude_exact) {
  longitude_exact_ = longitude_exact;
}

void GPSCoordinate::set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact) {
  latitude_exact_ = latitude_exact;
  longitude_exact_ = longitude_exact;
}

std::string GPSCoordinate::to_string() const {
  std::stringstream coord_str;
  coord_str << "("
            << to_string_helper(latitude_exact_)
            << ", "
            << to_string_helper(longitude_exact_)
            << ")";
  return coord_str.str();
}

std::string GPSCoordinate::to_string_latitude() const {
  return to_string_helper(latitude_exact_);
}

std::string GPSCoordinate::to_string_longitude() const {
  return to_string_helper(longitude_exact_);
}

int GPSCoordinate::round_to_int_latitude() const {
  return round_to_helper(latitude_exact_);
}

int GPSCoordinate::round_to_int_longitude() const {
  return round_to_helper(longitude_exact_);
}

std::string GPSCoordinate::to_string_helper(int32_t input) {
  std::stringstream coord_str;

  int input_part_whole = input / kExactCoordinateScaleFactor;
  int input_part_fractional = abs(input) % kExactCoordinateScaleFactor;

  coord_str << std::setfill('0')
            << input_part_whole << "." << std::setw(kExactCoordinateDecimalPlaces) << input_part_fractional;

  return coord_str.str();
}

int GPSCoordinate::round_to_helper(int32_t input) {
  int input_part_whole = input / kExactCoordinateScaleFactor;
  int input_part_fractional = abs(input) % kExactCoordinateScaleFactor;

  if ((input_part_fractional / static_cast<int>(pow(10, kExactCoordinateDecimalPlaces-1))) < 5) {
    return input_part_whole;
  }
  return input_part_whole > 0 ? input_part_whole + 1 : input_part_whole - 1;
}
