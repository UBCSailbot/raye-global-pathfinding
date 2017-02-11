// Copyright 2016 UBC Sailbot

#ifndef DATATYPES_GPSCOORDINATE_H_
#define DATATYPES_GPSCOORDINATE_H_

#include <cstdint>

class GPSCoordinate {
 public:
  static constexpr int32_t kExactCoordinateScaleFactor = static_cast<int32_t> (10000000);

  enum WaypointLevel {
    micro, local, global
  };

  GPSCoordinate();
  GPSCoordinate(int32_t latitude, int32_t longitude, WaypointLevel waypoint_level);

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
   * @return Exact latitude in degrees * kExactCoordinateScaleFactor
   */
  int32_t latitude_exact() const { return latitude_exact_; }
  /**
   * @return Exact longitude in degrees * kExactCoordinateScaleFactor
   */
  int32_t longitude_exact() const { return longitude_exact_; }
  /**
   * @return The waypoint level - from which pathfinder the waypoint originated. Either global, local, or micro.
   */
  WaypointLevel waypoint_level() const { return waypoint_level_; }

  virtual void set_lat_exact(int32_t latitude_exact);
  virtual void set_lng_exact(int32_t longitude_exact);
  virtual void set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact);
  virtual void set_waypoint_level(WaypointLevel waypoint_level);

 protected:
  /// Latitude in degrees * kExactCoordinateScaleFactor
  int32_t latitude_exact_ = 0;
  /// Longitude in degrees * kExactCoordinateScaleFactor
  int32_t longitude_exact_ = 0;

  /// Default to global
  WaypointLevel waypoint_level_ = WaypointLevel::global;
};

#endif  // DATATYPES_GPSCOORDINATE_H_
