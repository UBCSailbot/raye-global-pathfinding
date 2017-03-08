// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICCOSTCALCULATORTEST_H_
#define PATHFINDING_BASICCOSTCALCULATORTEST_H_

#include <gtest/gtest.h>
#include <pathfinding/BasicCostCalculator.h>
#include <pathfinding/BasicHexMap.h>
#include <planet/HexPlanet.h>

class BasicCostCalculatorTest : public ::testing::Test {
 protected:
  HexPlanet planet_;
  BasicCostCalculatorTest();
};

#endif  // PATHFINDING_BASICCOSTCALCULATORTEST_H_
