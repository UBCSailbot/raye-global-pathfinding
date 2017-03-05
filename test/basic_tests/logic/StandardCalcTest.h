// Copyright 2017 UBC Sailbot

#ifndef LOGIC_STANDARDCALCTEST_H_
#define LOGIC_STANDARDCALCTEST_H_

#include <Eigen/Dense>
#include <gtest/gtest.h>

#include <common/GeneralDefs.h>
#include <datatypes/GPSCoordinateFast.h>

class StandardCalcTest : public ::testing::Test  {
 private:
  /**
  * Creates an Eigen::Vector3f object from input coordinates
  * @param x (float)
  * @param y (float)
  * @param z (float)
  * @return Eigen::Vector3f
  */
  Eigen::Vector3f create_point(float x, float y, float z, float r = 1.0) const;

  /**
   * Creates a GPSCoordinateFast object from latitude and longitude pair
   * @param latitude (in degrees)
   * @param longitude (in degrees)
   * @return GPSCoordinateFast
   */
  GPSCoordinateFast create_coordinate(double latitude, double longitude) const;

 protected:
  /**
   * Creates a string representation of the input point
   * @param point (Eigen::Vector3f)
   * @return string representation
   */
  std::string to_string(Eigen::Vector3f point);

  /**
   * test_point_data >> (test point GPS coordinate, test point cartesian coordinate)
   */
  typedef std::pair<const GPSCoordinateFast, const Eigen::Vector3f> conversion_test_point_data_t;
  /**
   * test_point >> (test point description, test_point_data)
   */
  typedef std::map<std::string, conversion_test_point_data_t> conversion_test_point_t;
  /**
   * iterator type for test_point
   */
  typedef conversion_test_point_t::iterator it_t;

  /**
   * test points for conversion test
   */
  // TODO(puya): add more test points from within the quadrants and not on surface
  conversion_test_point_t conversion_test_points_{
      {"Origin"            , {create_coordinate(0  ,   0), create_point(1 ,  0,  0)}},
      {"Opposite to Origin", {create_coordinate(0  , 180), create_point(-1,  0,  0)}},
      {"North Pole"        , {create_coordinate(90 ,   0), create_point(0 ,  0,  1)}},
      {"South Pole"        , {create_coordinate(-90,   0), create_point(0 ,  0, -1)}},
      {"East on Equator"   , {create_coordinate(0  ,  90), create_point(0 ,  1,  0)}},
      {"West on Equator"   , {create_coordinate(0  , -90), create_point(0 , -1,  0)}},
  };
};

#endif  // LOGIC_STANDARDCALCTEST_H_


