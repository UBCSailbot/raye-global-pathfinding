// Copyright 2017 UBC Sailbot

#include "logic/StandardCalcTest.h"

#include <string>
#include <map>

#include <logic/StandardCalc.h>

Eigen::Vector3f StandardCalcTest::create_point(float x, float y, float z, float r) const {
  Eigen::Vector3f point(r * x, r * y, r * z);
  return point;
}

GPSCoordinateFast StandardCalcTest::create_coordinate(double latitude, double longitude) const {
  GPSCoordinateFast coordinate(static_cast<int32_t>(latitude * GPSCoordinate::kExactCoordinateScaleFactor),
                               static_cast<int32_t>(longitude * GPSCoordinate::kExactCoordinateScaleFactor));
  return coordinate;
}

std::string StandardCalcTest::to_string(Eigen::Vector3f point) {
  std::string point_str;
  point_str = "(" + std::to_string(point(0)) + ", " + std::to_string(point(1)) + ", " + std::to_string(point(2)) + ")";
  return point_str;
}

TEST_F(StandardCalcTest, CoordToPoint) {
  for (it_t it = conversion_test_points_.begin(); it != conversion_test_points_.end(); ++it) {
    // For each test in test_points:
    std::string                  description = it->first;
    conversion_test_point_data_t data        = it->second;
    const GPSCoordinateFast      coordinate  = data.first;
    const Eigen::Vector3f        cartesian   = data.second;
    Eigen::Vector3f              converted(0, 0, 0);

    standard_calc::CoordToPoint(coordinate, &converted);

    EXPECT_TRUE(standard_calc::almost_equal(cartesian, converted))
              << "\nTest point >>>>> '" << description << "' <<<<< failed.\n"
              << "Expected: " << to_string(cartesian) << "\t Converted: " << to_string(converted) << "\n";
  }
}

TEST_F(StandardCalcTest, PointToCoord) {
  for (it_t it = conversion_test_points_.begin(); it != conversion_test_points_.end(); ++it) {
    // For each test in test_points:
    std::string                  description = it->first;
    conversion_test_point_data_t data        = it->second;
    const GPSCoordinateFast      coordinate  = data.first;
    const Eigen::Vector3f        cartesian   = data.second;
    GPSCoordinateFast            converted(0, 0);

    standard_calc::PointToCoord(cartesian, &converted);

    EXPECT_TRUE(coordinate.almost_equal(converted))
              << "\nTest point >>>>> '" << description << "' <<<<< failed.\n"
              << "Expected: " << coordinate.to_string() << "\t Converted: " << converted.to_string() << "\n";
  }
}

