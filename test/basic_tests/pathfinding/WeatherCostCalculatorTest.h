// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_WEATHERCOSTCALCULATORTEST_H_
#define PATHFINDING_WEATHERCOSTCALCULATORTEST_H_

#include <gtest/gtest.h>
#include <pathfinding/WeatherCostCalculator.h>
#include <pathfinding/WeatherHexMap.h>
#include <planet/HexPlanet.h>

class WeatherCostCalculatorTest : public ::testing::Test {
 protected:
  HexPlanet planet_;
  WeatherCostCalculatorTest();
};

#endif  // PATHFINDING_WEATHERCOSTCALCULATORTEST_H_
