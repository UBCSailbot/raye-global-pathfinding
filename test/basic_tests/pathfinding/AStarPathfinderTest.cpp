// Copyright 2017 UBC Sailbot

#include "AStarPathfinderTest.h"
#include <pathfinding/AStarPathfinder.h>

AStarPathfinderTest::AStarPathfinderTest() : planet_1_(1), planet_2_(2), planet_3_(3), planet_4_(4) {}

TEST_F(AStarPathfinderTest, ReturnsEmptyPathForSameTarget) {
  HaversineHeuristic heuristic_;
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, &heuristic_, &cost_calculator_, 0, 0);
  auto result = pathfinder.Run();

  EXPECT_EQ(result.size(), static_cast<size_t>(1));
  EXPECT_EQ(result[0],  static_cast<HexVertexId>(0));
}

TEST_F(AStarPathfinderTest, ReturnPathSizeOne) {
  HaversineHeuristic heuristic_;
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, &heuristic_, &cost_calculator_, 0, 32);
  auto result = pathfinder.Run();

  EXPECT_EQ(result.size(), static_cast<size_t>(2));
  EXPECT_EQ(result[0], static_cast<HexVertexId>(0));
  EXPECT_EQ(result[1], static_cast<HexVertexId>(32));
}

TEST_F(AStarPathfinderTest, shouldFindSameLengthPathBothWays) {
  HaversineHeuristic heuristic_;
  NaiveCostCalculator cost_calculator_(planet_2_);

  AStarPathfinder pathfinder1(planet_2_, &heuristic_, &cost_calculator_, 0, 90);
  auto result1 = pathfinder1.Run();

  AStarPathfinder pathfinder2(planet_2_, &heuristic_, &cost_calculator_, 90, 0);
  auto result2 = pathfinder2.Run();

  EXPECT_GT(result1.size(), static_cast<size_t>(2));
  EXPECT_GT(result2.size(), static_cast<size_t>(2));
  EXPECT_EQ(result2.size(), result1.size());
}

// TODO(denist) Add tests to ensure optimal path is found

