// Copyright 2016 UBC Sailbot

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

bool GPSCoordinate::operator==(const GPSCoordinate &other) const {
  return latitude_exact_ == other.latitude_exact() && longitude_exact_ == other.longitude_exact();
}

bool GPSCoordinate::operator!=(const GPSCoordinate &other) const {
  return latitude_exact_ != other.latitude_exact() || longitude_exact_ != other.longitude_exact();
}

std::string GPSCoordinate::to_string() const {
  std::string coord_str;
  coord_str = "(" + std::to_string(latitude_exact_) + ", " + std::to_string(longitude_exact_) + ")";
  return coord_str;
}
