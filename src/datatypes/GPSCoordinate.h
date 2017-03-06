// Copyright 2016 UBC Sailbot

#ifndef DATATYPES_GPSCOORDINATE_H_
#define DATATYPES_GPSCOORDINATE_H_

#include <cstdint>
#include <string>

class GPSCoordinate {
 public:
  static constexpr int32_t kExactCoordinateScaleFactor = 10000000;
  /// The number of decimal places the scaling factor allows for. Should be equal to log10(kExactCoordinateScaleFactor)
  static constexpr int32_t kExactCoordinateDecimalPlaces = 7;

  GPSCoordinate();
  /**
   * @param latitude_exact Latitude in degrees * kExactCoordinateScaleFactor
   * @param longitude_exact Longitude in degrees * kExactCoordinateScaleFactor
   */
  GPSCoordinate(int32_t latitude_exact, int32_t longitude_exact);

  /**
   * @param other Other GPS Coordinate.
   * @return Whether two GPS Coordinates represent the exact same position.
   */
  bool operator==(const GPSCoordinate &other) const;
  /**
   * @param other Other GPS Coordinate.
   * @return Whether two GPS Coordinates do not represent the exact same position.
   */
  bool operator!=(const GPSCoordinate &other) const;

  /**
   * Returns a well-formatted string representation of the exact coordinate, (lat, lng)
   * Note: Values are in degrees.
   * @return String representation of coordinate.
   */
  std::string to_string() const;

  /**
   * Returns a well-formatted string representation of the exact latitude
   * Note: Value is in degrees.
   * @return String representation of coordinate.
   */
  std::string to_string_latitude() const;

  /**
   * Returns a well-formatted string representation of the exact longitude
   * Note: Value is in degrees.
   * @return String representation of coordinate.
   */
  std::string to_string_longitude() const;

  /**
   * @return Exact latitude in degrees * kExactCoordinateScaleFactor
   */
  int32_t latitude_exact() const { return latitude_exact_; }
  /**
   * @return Exact longitude in degrees * kExactCoordinateScaleFactor
   */
  int32_t longitude_exact() const { return longitude_exact_; }

  virtual void set_lat_exact(int32_t latitude_exact);
  virtual void set_lng_exact(int32_t longitude_exact);
  virtual void set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact);

 protected:
  /// Latitude in degrees * kExactCoordinateScaleFactor
  int32_t latitude_exact_ = 0;
  /// Longitude in degrees * kExactCoordinateScaleFactor
  int32_t longitude_exact_ = 0;

  /**
   * Creates a string decimal point representation of an "exact" coordinate component.
   * @param input Number to format.
   * @return Fixed point string.
   */
  static std::string to_string_helper(int32_t input);
};

#endif  // DATATYPES_GPSCOORDINATE_H_
