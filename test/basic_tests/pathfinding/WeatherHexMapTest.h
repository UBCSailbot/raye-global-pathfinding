// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_WEATHERHEXMAPTEST_H_
#define PATHFINDING_WEATHERHEXMAPTEST_H_

#include <gtest/gtest.h>
#include <pathfinding/WeatherHexMap.h>
#include <planet/HexPlanet.h>

class WeatherHexMapTest : public ::testing::Test {
 protected:
  WeatherHexMapTest();
  HexPlanet planet_1_;
};

#endif  // PATHFINDING_WEATHERHEXMAPTEST_H_
