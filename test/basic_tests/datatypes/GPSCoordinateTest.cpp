// Copyright 2017 UBC Sailbot

#include "datatypes/GPSCoordinateTest.h"

#include <datatypes/GPSCoordinateFast.h>

GPSCoordinateTest::GPSCoordinateTest() {}

TEST_F(GPSCoordinateTest, TestEmptyConstructor) {
  GPSCoordinateFast coordinate = GPSCoordinateFast();

  EXPECT_EQ(0., coordinate.latitude());
  EXPECT_EQ(0, coordinate.latitude_exact());
  EXPECT_EQ(0., coordinate.longitude());
  EXPECT_EQ(0, coordinate.longitude_exact());
}

TEST_F(GPSCoordinateTest, TestRadianConstructor) {
//  GPSCoordinateFast coordinate = GPSCoordinateFast(0, 0);
//
//  EXPECT_EQ(0., coordinate.latitude());
//  EXPECT_EQ(0, coordinate.latitude_exact());
//  EXPECT_EQ(0., coordinate.longitude());
//  EXPECT_EQ(0, coordinate.longitude_exact());
//
//  coordinate = GPSCoordinateFast(M_PI, M_PI);
//
//  EXPECT_EQ(M_PI, coordinate.latitude());
//  EXPECT_EQ(1800000000, coordinate.latitude_exact());
//  EXPECT_EQ(M_PI, coordinate.longitude());
//  EXPECT_EQ(1800000000, coordinate.longitude_exact());
//
//  coordinate = GPSCoordinateFast(M_PI / 2, M_PI / 2);
//
//  EXPECT_EQ(M_PI / 2, coordinate.latitude());
//  EXPECT_EQ(900000000, coordinate.latitude_exact());
//  EXPECT_EQ(M_PI / 2, coordinate.longitude());
//  EXPECT_EQ(900000000, coordinate.longitude_exact());
}

TEST_F(GPSCoordinateTest, TestExactConstructor) {
  GPSCoordinateFast coordinate = GPSCoordinateFast(1800000000, -1800000000);

  EXPECT_EQ(M_PI, coordinate.latitude());
  EXPECT_EQ(1800000000, coordinate.latitude_exact());
  EXPECT_EQ(-M_PI, coordinate.longitude());
  EXPECT_EQ(-1800000000, coordinate.longitude_exact());

  coordinate = GPSCoordinateFast(-900000000, 900000000);

  EXPECT_EQ(-M_PI / 2, coordinate.latitude());
  EXPECT_EQ(-900000000, coordinate.latitude_exact());
  EXPECT_EQ(M_PI / 2, coordinate.longitude());
  EXPECT_EQ(900000000, coordinate.longitude_exact());
}
