// Copyright 2016 UBC Sailbot

#include "datatypes/GPSCoordinate.h"

GPSCoordinate::GPSCoordinate() {}

GPSCoordinate::GPSCoordinate(int32_t latitude_exact, int32_t longitude_exact, WaypointLevel waypoint_level)
    : latitude_exact_(latitude_exact), longitude_exact_(longitude_exact), waypoint_level_(waypoint_level) {}

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

void GPSCoordinate::set_waypoint_level(WaypointLevel waypoint_level) {
  waypoint_level_ = waypoint_level;
}

bool GPSCoordinate::operator==(const GPSCoordinate &other) const {
  return latitude_exact_ == other.latitude_exact() && longitude_exact_ == other.longitude_exact();
}

bool GPSCoordinate::operator!=(const GPSCoordinate &other) const {
  return latitude_exact_ != other.latitude_exact() || longitude_exact_ != other.longitude_exact();
}

