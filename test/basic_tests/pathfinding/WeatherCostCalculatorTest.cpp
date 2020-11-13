// Copyright 2017 UBC Sailbot

#include "WeatherCostCalculatorTest.h"
#include <logic/StandardCalc.h>

/// Size of planet used in WeatherCostCalculatorTests
static constexpr size_t kSizeOfTestPlanet = 4;

/// Number of time steps used to create test WeatherHexMap
static constexpr uint32_t kTimeSteps = 10;

/// Default travel time to use in cost calculations
static constexpr uint32_t kTravelTime = 1;

WeatherCostCalculatorTest::WeatherCostCalculatorTest() : planet_(kSizeOfTestPlanet) {}

/**
 * Test that cost is calculated correctly when calculator is given two valid
 * vertex IDs.
 */
TEST_F(WeatherCostCalculatorTest, CalculateCostForValidVerticesTest) {
  auto map = std::make_unique<WeatherHexMap>(planet_, kTimeSteps, 48, 235, 21, 203);
  WeatherCostCalculator calculator(planet_, map, 0);

  std::srand(static_cast<uint>(std::time(0)));
  HexVertexId num_vertices = static_cast<HexVertexId>(planet_.vertex_count());
  HexVertexId vertex_a = (std::rand() % num_vertices);
  HexVertexId vertex_b = (std::rand() % num_vertices);

  auto a_to_b = calculator.calculate_target(vertex_a, vertex_b, kTravelTime);
  auto b_to_a = calculator.calculate_target(vertex_b, vertex_a, kTravelTime);
  EXPECT_EQ(a_to_b.cost, b_to_a.cost);

  auto a_to_a = calculator.calculate_target(vertex_a, vertex_a, kTravelTime);
  auto b_to_b = calculator.calculate_target(vertex_b, vertex_b, kTravelTime);
  auto distance = standard_calc::DistBetweenTwoCoords(planet_.vertex(vertex_a).coordinate,
                                                      planet_.vertex(vertex_b).coordinate);
  EXPECT_EQ(a_to_a.cost / 2 + b_to_b.cost / 2 + distance, a_to_b.cost);
}

/**
 * Test that an invalid cost value is correctly returned when calculator is
 * given two vertex IDs of which at least one is invalid.
 */
TEST_F(WeatherCostCalculatorTest, CalculateCostForInvalidVerticesTest) {
  auto map = std::make_unique<WeatherHexMap>(planet_, kTimeSteps, 48, 235, 21, 203);
  WeatherCostCalculator calculator(planet_, map, 0);

  HexVertexId num_vertices = static_cast<HexVertexId>(planet_.vertex_count());

  std::srand(static_cast<uint>(std::time(0)));
  HexVertexId valid_id = (std::rand() % num_vertices);  // A valid vertex ID
  HexVertexId invalid_id = num_vertices + 1;  // An invalid vertex ID

  EXPECT_THROW(calculator.calculate_target(valid_id, invalid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate_target(invalid_id, valid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate_target(kInvalidHexVertexId, valid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate_target(valid_id, kInvalidHexVertexId, kTravelTime),
               std::runtime_error);
}
