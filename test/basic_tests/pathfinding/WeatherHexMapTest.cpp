// Copyright 2017 UBC Sailbot

#include "WeatherHexMapTest.h"

WeatherHexMapTest::WeatherHexMapTest() : planet_1_(1) {}

/// The number of time steps used to generate the test |WeatherHexMap|
static constexpr uint32_t kTimeSteps = 10;

/**
 * Test stored |WeatherDatum|s.
 */
TEST_F(WeatherHexMapTest, WeatherHexMapInitializationTest) {
  WeatherHexMap map1(planet_1_, kTimeSteps);

  // it's probably excessive to test all of them, so only test a few edge cases
  EXPECT_NO_THROW(map1.get_weather(0, 0));
  EXPECT_NO_THROW(map1.get_weather(static_cast<HexVertexId>(planet_1_.vertex_count() - 1), 0));
  EXPECT_NO_THROW(map1.get_weather(0, kTimeSteps - 1));
  EXPECT_NO_THROW(map1.get_weather(static_cast<HexVertexId>(planet_1_.vertex_count() - 1), kTimeSteps - 1));
}

/**
 * Test that a runtime error is correctly thrown when querying for weather
 * data for nonexistent vertices or invalid time steps
 */
TEST_F(WeatherHexMapTest, GetRiskForInvalidVertexOrTimeTest) {
  WeatherHexMap map(planet_1_, kTimeSteps);

  EXPECT_THROW(map.get_weather(kInvalidHexVertexId, 0), std::runtime_error);
  EXPECT_THROW(map.get_weather(static_cast<HexVertexId>(planet_1_.vertex_count()), 0), std::runtime_error);

  EXPECT_THROW(map.get_weather(0, -1), std::runtime_error);
  EXPECT_THROW(map.get_weather(0, kTimeSteps), std::runtime_error);
}
