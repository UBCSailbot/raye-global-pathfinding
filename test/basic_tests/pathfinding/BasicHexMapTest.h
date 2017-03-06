// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICHEXMAPTEST_H_
#define PATHFINDING_BASICHEXMAPTEST_H_

#include <gtest/gtest.h>
#include <pathfinding/BasicHexMap.h>
#include <planet/HexPlanet.h>

class BasicHexMapTest : public ::testing::Test {
 protected:
  BasicHexMapTest();
  HexPlanet planet_1_;
};


#endif  // PATHFINDING_BASICHEXMAPTEST_H_
