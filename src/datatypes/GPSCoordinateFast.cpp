// Copyright 2017 UBC Sailbot

#include "datatypes/GPSCoordinateFast.h"

#include <logic/StandardCalc.h>

GPSCoordinateFast::GPSCoordinateFast() {}
GPSCoordinateFast::GPSCoordinateFast(int32_t latitude_exact, int32_t longitude_exact) {
  set_lat_lng_exact(latitude_exact, longitude_exact);
}
GPSCoordinateFast::GPSCoordinateFast(double latitude, double longitude) {
  set_lat_lng(latitude, longitude);
}

void GPSCoordinateFast::set_lat_exact(int32_t latitude_exact) {
  latitude_exact_ = latitude_exact;
  latitude_ = standard_calc::deg_to_rad(latitude_exact_ / static_cast<float>(kExactCoordinateScaleFactor));
}

void GPSCoordinateFast::set_lng_exact(int32_t longitude_exact) {
  longitude_exact_ = longitude_exact;
  longitude_ = standard_calc::deg_to_rad(longitude_exact_ / static_cast<float>(kExactCoordinateScaleFactor));
}

void GPSCoordinateFast::set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact) {
  set_lat_exact(latitude_exact);
  set_lng_exact(longitude_exact);
}

void GPSCoordinateFast::set_lat_lng(double lat, double lng) {
  latitude_ = lat;
  longitude_ = lng;
  longitude_exact_ =
      static_cast<uint32_t >(standard_calc::rad_to_deg(longitude_) * (kExactCoordinateScaleFactor));
  latitude_exact_ =
      static_cast<uint32_t >(standard_calc::rad_to_deg(latitude_) * (kExactCoordinateScaleFactor));
}

bool GPSCoordinateFast::almost_equal(const GPSCoordinateFast &other, const double tolerance) const {
  // L1 norm of the difference between the two coordinates
  double coord_diff_l1 = fabs(latitude_ - other.latitude()) + fabs(longitude_ - other.longitude());
  return (coord_diff_l1 < tolerance);
}
