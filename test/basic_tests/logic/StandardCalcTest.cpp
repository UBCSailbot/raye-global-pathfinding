// Copyright 2017 UBC Sailbot

#include "logic/StandardCalcTest.h"

#include <array>

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
  for (const auto &test : conversion_test_points_) {
    // For each test in test_points:
    const std::string &description = test.first;
    const conversion_test_point_data_t &data = test.second;
    const GPSCoordinateFast &coordinate = data.first;
    const Eigen::Vector3f &cartesian = data.second;

    Eigen::Vector3f converted(0, 0, 0);
    standard_calc::CoordToPoint(coordinate, &converted);

    EXPECT_TRUE(standard_calc::almost_equal(cartesian, converted))
              << std::endl << "Test point >>>>> '" << description << "' <<<<< failed." << std::endl
              << "Expected: " << to_string(cartesian) << "\t Converted: " << to_string(converted) << std::endl;
  }
}

TEST_F(StandardCalcTest, PointToCoord) {
  for (const auto &test : conversion_test_points_) {
    // For each test in test_points:
    const std::string &description = test.first;
    const conversion_test_point_data_t &data = test.second;
    const GPSCoordinateFast &coordinate = data.first;
    const Eigen::Vector3f &cartesian = data.second;

    GPSCoordinateFast converted(0, 0);
    standard_calc::PointToCoord(cartesian, &converted);

    EXPECT_TRUE(coordinate.almost_equal(converted))
              << std::endl << "Test point >>>>> '" << description << "' <<<<< failed." << std::endl
              << "Expected: " << coordinate.to_string() << "\t Converted: " << converted.to_string() << std::endl;
  }
}

TEST_F(StandardCalcTest, DistBetweenTwoCoords) {
  // Tolerance for distance tests in meters.
  constexpr uint32_t kDistanceTolerance = 1;

  struct Test {
    uint32_t output;
    GPSCoordinateFast source, target;
  };

  const std::vector<Test> test_data = {
      {157249, GPSCoordinateFast(0, 0), GPSCoordinateFast(10000000, 10000000)},
      {7871769, GPSCoordinateFast(350000000, 450000000), GPSCoordinateFast(350000000, 1350000000)},
      {10741424, GPSCoordinateFast(-600000000, 300000000), GPSCoordinateFast(340000000, 30000000)},
      {1869, GPSCoordinateFast(492798640, -1231915190), GPSCoordinateFast(492804800, -1231657700)}
  };

  for (const auto &test : test_data) {
    uint32_t distance = standard_calc::DistBetweenTwoCoords(test.source, test.target);
    EXPECT_NEAR(test.output, distance, kDistanceTolerance)
              << "Test DistBetweenTwoCoords(" << test.source.to_string() << ", " << test.target.to_string()
              << ") failed." << std::endl;
  }
}

TEST_F(StandardCalcTest, BearingBetweenTwoCoords) {
  // Tolerance for angle tests in degrees.
  constexpr double kAngleTolerance = 0.5;

  struct Test {
    double output;
    GPSCoordinateFast source, target;
  };

  const std::vector<Test> test_data = {
      {60, GPSCoordinateFast(350000000, 450000000), GPSCoordinateFast(350000000, 1350000000)},
      {-60, GPSCoordinateFast(350000000, 1350000000), GPSCoordinateFast(350000000, 450000000)},
      {45, GPSCoordinateFast(0, 0), GPSCoordinateFast(10000000, 10000000)},
      {-135, GPSCoordinateFast(10000000, 10000000), GPSCoordinateFast(0, 0)}
  };

  for (const auto &test : test_data) {
    double angle = standard_calc::BearingBetweenTwoCoords(test.source, test.target);
    EXPECT_NEAR(test.output, angle, kAngleTolerance)
              << "Test BearingBetweenTwoCoords(" << test.source.to_string() << ", " << test.target.to_string()
              << ") failed." << std::endl;
  }
}

TEST_F(StandardCalcTest, BoundTo180) {
  // Tolerance for angle tests in degrees.
  constexpr double kAngleTolerance = 0.00001;

  // bounded, unbounded
  const std::vector<std::pair<double, double>> test_data = {
      // No change
      {5, 5},
      {-1, -1},
      {-0.00001, -0.00001},
      {-179.00001, -179.00001},
      {179.99999, 179.99999},
      {180, 180},
      {-90, -90},
      {-135, -135},
      {90, 90},
      {135, 135},
      // Greater than 180
      {-160, 200},
      {180, -180},
      // Less than -180
      {160, -200},
      // Greater than 360 positive
      {170, 890},
      // Greater than 360 negative
      {-160, 920},
      // Less than -360 to positive
      {160, -920},
      // Less than -360 to negative
      {-170, -890},
  };

  for (const auto &test : test_data) {
    double expected = test.first;
    double angle = standard_calc::BoundTo180(test.second);
    EXPECT_NEAR(expected, angle, kAngleTolerance)
              << "Test BoundTo180(" << test.second << ") failed." << std::endl;
  }
}

TEST_F(StandardCalcTest, IsAngleBetween) {
  struct Test {
    bool output;
    double first, middle, second;
  };

  const std::vector<Test> test_data = {
      {true, 0, 10, 30},
      {true, -30, 0, 30},
      {true, 30, 0, -30},
      {true, -100, -50, -30},
      {true, 160, 170, -160},
      {true, -160, -170, 160},

      {true, -1, 0, 1},
      {true, 1, 0, -1},

      {true, -179, 180, 179},
      {true, 179, 180, -179},

      {false, 0, 0, 100},
      {false, -100, -50, -70},
      {false, 0, 1, 0},
      {false, 30, 20, 50},
      {false, -45, -170, 45},

      {true, 340, 0, 380},
      {false, 160, 0, -100},
      {false, -45, 45, 45},
  };

  for (const auto &test : test_data) {
    double actual = standard_calc::IsAngleBetween(test.first, test.middle, test.second);
    EXPECT_EQ(test.output, actual)
              << "Test IsAngleBetween(" << test.first << ", " << test.middle << ", " << test.second << ") failed."
              << std::endl;
  }
}

TEST_F(StandardCalcTest, CalculateAngleDelta) {
  // Tolerance for angle tests in degrees.
  constexpr double kAngleTolerance = 0.00001;

  struct Test {
    double output, source, target;
  };

  const std::vector<Test> test_data = {
      {0, 0, 0},
      {-30, 50, 20},
      {30, -50, -20},
      {70, -50, 20},
      {60, 150, -150},
      {-60, -150, 150},
      {0, -180, -180},
      {0, -180, 180},
      {-1, -179, 180},
      {1, 180, -179},
      {1, 179, -180},
      {2, 179, -179},
      {-2, -179, 179},
      {2, -1, 1},
      {-2, 1, -1},
      {2, -91, -89},
      {-2, -89, -91},
      {-2, 91, 89},
      {2, 89, 91},
      {-2, -134, -136},
      {2, -136, -134},
      {2, 134, 136},
      {-2, 136, 134},
  };

  for (const auto &test : test_data) {
    double angle = standard_calc::calculate_angle_delta(test.source, test.target);
    EXPECT_NEAR(test.output, angle, kAngleTolerance)
              << "Test calculate_angle_delta(" << test.source << ", " << test.target << ") failed." << std::endl;
  }
}
