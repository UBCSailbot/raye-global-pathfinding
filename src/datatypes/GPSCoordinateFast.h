// Copyright 2016 UBC Sailbot

#ifndef DATATYPES_GPSCOORDINATEFAST_H_
#define DATATYPES_GPSCOORDINATEFAST_H_

#include "datatypes/GPSCoordinate.h"

/**
 * @brief GPS Coordinate that keeps a radian version of itself for performing faster calculations.
 */
class GPSCoordinateFast: public GPSCoordinate {
 public:
  GPSCoordinateFast();
  GPSCoordinateFast(int32_t latitude_exact, int32_t longitude_exact);
  explicit GPSCoordinateFast(const GPSCoordinate &coordinate);

  void set_lat_exact(int32_t latitude_exact);
  void set_lng_exact(int32_t longitude_exact);
  void set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact);
  void set_waypoint(const GPSCoordinate &coordinate);

  /**
   * @return Latitude in radians
   */
  double latitude() const { return latitude_; }
  /**
   * @return Longitude in radians
   */
  double longitude() const { return longitude_; }

 private:
  /// Latitude in radians
  double latitude_ = 0;
  /// Longitude in radians
  double longitude_ = 0;
};


#endif  // DATATYPES_GPSCOORDINATEFAST_H_
