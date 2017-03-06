// Copyright 2016 UBC Sailbot

#ifndef DATATYPES_GPSCOORDINATEFAST_H_
#define DATATYPES_GPSCOORDINATEFAST_H_

#include <cmath>

#include "datatypes/GPSCoordinate.h"

/**
 * @brief GPS Coordinate that keeps a radian version of itself for performing faster calculations.
 */
class GPSCoordinateFast: public GPSCoordinate {
 public:
  /**
   * The tolerance value in radians used to check if two gps coordinates are "almost equal"
   * Units: radians
   */
  static constexpr float kCoordTolerance = 0.01;

  GPSCoordinateFast();
  /**
   * @param latitude_exact Latitude in degrees * kExactCoordinateScaleFactor
   * @param longitude_exact Longitude in degrees * kExactCoordinateScaleFactor
   */
  GPSCoordinateFast(int32_t latitude_exact, int32_t longitude_exact);
  /**
   * @param latitude Latitude in radians
   * @param longitude Longitude in radians
   */
  GPSCoordinateFast(double latitude, double longitude);

  void set_lat_exact(int32_t latitude_exact);
  void set_lng_exact(int32_t longitude_exact);
  void set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact);

  void set_lat_lng(double lat, double lng);

  /**
   * Checks if other coordinate is (within tolerance) equal to the coordinate object
   * Tolerance is tolerancePercentage * scale (e.g. 1% * 180 >> 1% tolerance in degrees)
   * @param other coordinate
   * @param scale: used to convert percent tolerance to absolute tolerance
   * @return true if equal within tolerance, false otherwise
   */
  bool almost_equal(const GPSCoordinateFast &other, const double tolerance = kCoordTolerance) const;

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
