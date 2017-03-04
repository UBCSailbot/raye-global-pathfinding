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
   * The tolerance percentage used to check if two coordinates are "almost equal"
   * This value is dimensionless. To find absolute tolerance one needs a scaling factor:
   * tolerance = percentTolerance * scale
   */
  static constexpr float kCoordTolerancePercentage = 0.01;

  GPSCoordinateFast();
  GPSCoordinateFast(int32_t latitude_exact, int32_t longitude_exact);
  explicit GPSCoordinateFast(const GPSCoordinate &coordinate);

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
  bool almost_equal(const GPSCoordinateFast &other, const double scale = M_PI) const;

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
