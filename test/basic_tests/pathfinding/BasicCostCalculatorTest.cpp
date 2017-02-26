// Copyright 2017 UBC Sailbot

#include "BasicCostCalculatorTest.h"
#include <ctime>

/// Size of planet used in BasicCostCalculatorTests
static constexpr size_t kSizeOfTestPlanet = 4;

/// Default travel time to use in cost calculations
static constexpr uint32_t kTravelTime = 1;

BasicCostCalculatorTest::BasicCostCalculatorTest()
    : planet(HexPlanet(kSizeOfTestPlanet)) { }

/**
 * Test that cost is calculated correctly when calculator is given two
 * valid vertex IDs.
 */
TEST_F(BasicCostCalculatorTest, CalculateCostForValidVerticesTest) {
  BasicCostCalculator calculator(planet, new BasicHexMap(planet));

  std::srand(static_cast<uint>(std::time(0)));
  HexVertexId num_vertices = static_cast<HexVertexId>(planet.vertex_count());
  HexVertexId vertex_a = (std::rand() % num_vertices);
  HexVertexId vertex_b = (std::rand() % num_vertices);

  uint32_t a_to_b = calculator.calculate(vertex_a, vertex_b, kTravelTime);
  uint32_t b_to_a = calculator.calculate(vertex_b, vertex_a, kTravelTime);
  EXPECT_EQ(a_to_b, b_to_a);

  uint32_t a_to_a = calculator.calculate(vertex_a, vertex_a, kTravelTime);
  uint32_t b_to_b = calculator.calculate(vertex_b, vertex_b, kTravelTime);
  EXPECT_EQ(a_to_a / 2 + b_to_b / 2, a_to_b);
}

/**
 * Test that an invalid cost value is correctly returned when calculator is
 * given two vertex IDs of which at least one is invalid.
 */
TEST_F(BasicCostCalculatorTest, CalculateCostForInvalidVerticesTest) {
  BasicCostCalculator calculator(planet, new BasicHexMap(planet));

  std::srand(static_cast<uint>(std::time(0)));
  HexVertexId num_vertices = static_cast<HexVertexId>(planet.vertex_count());

  HexVertexId valid_id = (std::rand() % num_vertices);  // a valid vertex ID
  HexVertexId invalid_id = num_vertices + 1;  // an invalid vertex ID

  EXPECT_THROW(calculator.calculate(valid_id, invalid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate(invalid_id, valid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate(kInvalidHexVertexId, valid_id, kTravelTime),
               std::runtime_error);
  EXPECT_THROW(calculator.calculate(valid_id, kInvalidHexVertexId, kTravelTime),
               std::runtime_error);
}
