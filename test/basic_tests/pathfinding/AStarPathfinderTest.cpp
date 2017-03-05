// Copyright 2017 UBC Sailbot

#include "AStarPathfinderTest.h"

#include "pathfinding/MockCostCalculator.h"
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/NaiveHeuristic.h"
#include "common/GeneralDefs.h"

const std::array<HexVertexId, 6> AStarPathfinderTest::kTestPath1 = {1, 110, 111, 267, 171, 86};

AStarPathfinderTest::AStarPathfinderTest() :
    planet_1_(1), planet_2_(2), planet_3_(3), planet_4_(4) {}

TEST_F(AStarPathfinderTest, ReturnsEmptyPathForSameTarget) {
  HaversineHeuristic heuristic_;
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, &heuristic_, &cost_calculator_, 0, 0);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), static_cast<size_t>(1));
  EXPECT_EQ(result[0], static_cast<HexVertexId>(0));
}

TEST_F(AStarPathfinderTest, ReturnPathSizeTwo) {
  HaversineHeuristic heuristic_;
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, &heuristic_, &cost_calculator_, 0, 32);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), static_cast<size_t>(2));
  EXPECT_EQ(result[0], static_cast<HexVertexId>(0));
  EXPECT_EQ(result[1], static_cast<HexVertexId>(32));
}

TEST_F(AStarPathfinderTest, ShouldFindSameLengthPathBothWays) {
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

TEST_F(AStarPathfinderTest, FindsLowestCostPath) {
  NaiveHeuristic heuristic;
  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), 1});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), 1});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), 1});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), 1});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), 1});

  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, 2);
  AStarPathfinder pathfinder(planet_3_, &heuristic, &cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), static_cast<size_t>(6));
  EXPECT_EQ(result[0], kTestPath1[0]);
  EXPECT_EQ(result[1], kTestPath1[1]);
  EXPECT_EQ(result[2], kTestPath1[2]);
  EXPECT_EQ(result[3], kTestPath1[3]);
  EXPECT_EQ(result[4], kTestPath1[4]);
  EXPECT_EQ(result[5], kTestPath1[5]);
}

TEST_F(AStarPathfinderTest, WillEventuallyFindTheWay) {
  NaiveHeuristic heuristic;
  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), 19});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), 1});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), 1});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), 1});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), 1});

  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, 10);
  AStarPathfinder pathfinder(planet_3_, &heuristic, &cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), kTestPath1.size());
  EXPECT_EQ(result[0], kTestPath1[0]);
  EXPECT_EQ(result[1], kTestPath1[1]);
  EXPECT_EQ(result[2], kTestPath1[2]);
  EXPECT_EQ(result[3], kTestPath1[3]);
  EXPECT_EQ(result[4], kTestPath1[4]);
  EXPECT_EQ(result[5], kTestPath1[5]);
}

TEST_F(AStarPathfinderTest, WillWaitAndFindLowCost) {
  NaiveHeuristic heuristic_;
  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), 30});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), 1});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), 1});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), 1});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), 1});
  for (auto id : kTestPath1) {
    const HexVertex &vertex = planet_3_.vertex(id);
    const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      auto neighbour = neighbours[i];
      if (std::find(kTestPath1.begin(), kTestPath1.end(), neighbour) == kTestPath1.end()) {
        for (size_t j = 0; j < kTestPath1.size(); ++j) {
          mock_cost_map.insert({std::make_tuple(neighbour, id, j), 40});
        }
      }
    }
  }

  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, 10);
  AStarPathfinder pathfinder(planet_3_, &heuristic_, &cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), kTestPath1.size());
  EXPECT_EQ(result[0], kTestPath1[0]);
  EXPECT_EQ(result[1], kTestPath1[1]);
  EXPECT_EQ(result[2], kTestPath1[2]);
  EXPECT_EQ(result[3], kTestPath1[3]);
  EXPECT_EQ(result[4], kTestPath1[4]);
  EXPECT_EQ(result[5], kTestPath1[5]);
}

TEST_F(AStarPathfinderTest, FindsLowestCostPathHaversine) {
  HaversineHeuristic heuristic;
  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), sailbot::kEarthRadius});

  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, sailbot::kEarthRadius + 10);
  AStarPathfinder pathfinder(planet_3_, &heuristic, &cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  ASSERT_EQ(result.size(), static_cast<size_t>(6));
  EXPECT_EQ(result[0], kTestPath1[0]);
  EXPECT_EQ(result[1], kTestPath1[1]);
  EXPECT_EQ(result[2], kTestPath1[2]);
  EXPECT_EQ(result[3], kTestPath1[3]);
  EXPECT_EQ(result[4], kTestPath1[4]);
  EXPECT_EQ(result[5], kTestPath1[5]);
}

