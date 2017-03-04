// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARPATHFINDERTEST_H_
#define PATHFINDING_ASTARPATHFINDERTEST_H_

#include <gtest/gtest.h>
#include <planet/HexPlanet.h>
#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/NaiveCostCalculator.h>

class AStarPathfinderTest : public ::testing::Test {
 protected:
  AStarPathfinderTest();
  HexPlanet planet_1_;
  HexPlanet planet_2_;
  HexPlanet planet_3_;
  HexPlanet planet_4_;
};

#endif  // PATHFINDING_ASTARPATHFINDERTEST_H_
