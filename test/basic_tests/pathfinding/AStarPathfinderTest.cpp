// Copyright 2017 UBC Sailbot

#include "AStarPathfinderTest.h"

#include "pathfinding/MockCostCalculator.h"
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/NaiveHeuristic.h"
#include "common/GeneralDefs.h"

const std::array<HexVertexId, 6> AStarPathfinderTest::kTestPath1 = {{1, 110, 111, 267, 171, 86}};

AStarPathfinderTest::AStarPathfinderTest() :
    planet_1_(1, 0), planet_2_(2, 0), planet_3_(3, 0), planet_4_(4, 0) {}

TEST_F(AStarPathfinderTest, ReturnsEmptyPathForSameTarget) {
  HaversineHeuristic heuristic_(planet_2_);
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, heuristic_, cost_calculator_, 0, 0);
  auto result = pathfinder.Run();

  EXPECT_EQ(0u, result.cost);
  EXPECT_EQ(0u, result.time);
  ASSERT_EQ(result.path.size(), static_cast<size_t>(1));
  EXPECT_EQ(result.path[0], static_cast<HexVertexId>(0));
}

TEST_F(AStarPathfinderTest, ReturnPathSizeTwo) {
  HaversineHeuristic heuristic_(planet_2_);
  NaiveCostCalculator cost_calculator_(planet_2_);
  AStarPathfinder pathfinder(planet_2_, heuristic_, cost_calculator_, 0, 32);
  auto result = pathfinder.Run();

  EXPECT_EQ(1u, result.cost);
  EXPECT_EQ(1u, result.time);
  ASSERT_EQ(result.path.size(), static_cast<size_t>(2));
  EXPECT_EQ(result.path[0], static_cast<HexVertexId>(0));
  EXPECT_EQ(result.path[1], static_cast<HexVertexId>(32));
}

TEST_F(AStarPathfinderTest, ShouldFindSameLengthPathBothWays) {
  HaversineHeuristic heuristic_(planet_2_);
  NaiveCostCalculator cost_calculator_(planet_2_);

  AStarPathfinder pathfinder1(planet_2_, heuristic_, cost_calculator_, 0, 90);
  auto result1 = pathfinder1.Run();

  AStarPathfinder pathfinder2(planet_2_, heuristic_, cost_calculator_, 90, 0);
  auto result2 = pathfinder2.Run();

  EXPECT_EQ(6u, result1.cost);
  EXPECT_EQ(6u, result1.time);
  EXPECT_EQ(6u, result2.cost);
  EXPECT_EQ(6u, result2.time);
  EXPECT_GT(result1.path.size(), static_cast<size_t>(2));
  EXPECT_GT(result2.path.size(), static_cast<size_t>(2));
  EXPECT_EQ(result2.path.size(), result1.path.size());
}

TEST_F(AStarPathfinderTest, FindsLowestCostPath) {
  NaiveHeuristic heuristic(planet_3_);

  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), 1});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), 1});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), 1});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), 1});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), 1});
  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, 2);

  AStarPathfinder pathfinder(planet_3_, heuristic, cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  EXPECT_EQ(5u, result.cost);
  EXPECT_EQ(5u, result.time);
  ASSERT_EQ(result.path.size(), static_cast<size_t>(6));
  EXPECT_EQ(result.path[0], kTestPath1[0]);
  EXPECT_EQ(result.path[1], kTestPath1[1]);
  EXPECT_EQ(result.path[2], kTestPath1[2]);
  EXPECT_EQ(result.path[3], kTestPath1[3]);
  EXPECT_EQ(result.path[4], kTestPath1[4]);
  EXPECT_EQ(result.path[5], kTestPath1[5]);
}

TEST_F(AStarPathfinderTest, WillEventuallyFindTheWay) {
  // Uses a valid but useless heuristic. Same behaviour as Dijkstra.
  // Warning: this runs in 3D (time) so it scales poorly, keep the size small.
  NaiveHeuristic heuristic(planet_3_, 0);

  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), 19});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), 1});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), 1});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), 1});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), 1});
  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, 10);

  AStarPathfinder pathfinder(planet_3_, heuristic, cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  EXPECT_EQ(23u, result.cost);
  EXPECT_EQ(5u, result.time);
  ASSERT_EQ(result.path.size(), kTestPath1.size());
  EXPECT_EQ(result.path[0], kTestPath1[0]);
  EXPECT_EQ(result.path[1], kTestPath1[1]);
  EXPECT_EQ(result.path[2], kTestPath1[2]);
  EXPECT_EQ(result.path[3], kTestPath1[3]);
  EXPECT_EQ(result.path[4], kTestPath1[4]);
  EXPECT_EQ(result.path[5], kTestPath1[5]);
}

// TODO(denis): Describe what this tests.
TEST_F(AStarPathfinderTest, WillWaitAndFindLowCost) {
  NaiveHeuristic heuristic_(planet_3_);

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
  AStarPathfinder pathfinder(planet_3_, heuristic_, cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  EXPECT_EQ(34u, result.cost);
  EXPECT_EQ(5u, result.time);
  ASSERT_EQ(result.path.size(), kTestPath1.size());
  EXPECT_EQ(result.path[0], kTestPath1[0]);
  EXPECT_EQ(result.path[1], kTestPath1[1]);
  EXPECT_EQ(result.path[2], kTestPath1[2]);
  EXPECT_EQ(result.path[3], kTestPath1[3]);
  EXPECT_EQ(result.path[4], kTestPath1[4]);
  EXPECT_EQ(result.path[5], kTestPath1[5]);
}

TEST_F(AStarPathfinderTest, FindsLowestCostPathHaversine) {
  HaversineHeuristic heuristic(planet_3_);

  MockCostCalculator::MockCostMap mock_cost_map;
  mock_cost_map.insert({std::make_tuple(1, 110, 0), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(110, 111, 1), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(111, 267, 2), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(267, 171, 3), sailbot::kEarthRadius});
  mock_cost_map.insert({std::make_tuple(171, 86, 4), sailbot::kEarthRadius});
  MockCostCalculator cost_calculator_(planet_3_, mock_cost_map, sailbot::kEarthRadius + 10);

  AStarPathfinder pathfinder(planet_3_, heuristic, cost_calculator_, 1, 86);
  auto result = pathfinder.Run();

  EXPECT_EQ(31855000u, result.cost);
  EXPECT_EQ(5u, result.time);
  ASSERT_EQ(result.path.size(), static_cast<size_t>(6));
  EXPECT_EQ(result.path[0], kTestPath1[0]);
  EXPECT_EQ(result.path[1], kTestPath1[1]);
  EXPECT_EQ(result.path[2], kTestPath1[2]);
  EXPECT_EQ(result.path[3], kTestPath1[3]);
  EXPECT_EQ(result.path[4], kTestPath1[4]);
  EXPECT_EQ(result.path[5], kTestPath1[5]);
}
