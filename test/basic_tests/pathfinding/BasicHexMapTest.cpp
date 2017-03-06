// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicHexMapTest.h"

BasicHexMapTest::BasicHexMapTest() : planet_1_(1) {}

/**
 * Test that the risk for all the vertices are within a valid range.
 */
TEST_F(BasicHexMapTest, RandomVertexRisksWithinBoundsTest) {
  BasicHexMap map1 = BasicHexMap::MakeRandom(planet_1_);

  for (HexVertexId i = 0; i < planet_1_.vertex_count(); ++i) {
    uint32_t vertex_risk = map1.get_risk(i);
    EXPECT_LE(vertex_risk, BasicHexMap::kDefaultMaxRisk);
    EXPECT_GE(vertex_risk, BasicHexMap::kDefaultRisk);
  }

  BasicHexMap map2 = BasicHexMap::MakeRandom(planet_1_, 10000, 1000000);

  for (HexVertexId i = 0; i < planet_1_.vertex_count(); ++i) {
    uint32_t vertex_risk = map2.get_risk(i);
    EXPECT_LE(vertex_risk, 1000000u);
    EXPECT_GE(vertex_risk, 10000u);
  }
}

/**
 * Test that an invalid risk value is correctly returned for vertex IDs that
 * do not exist on the planet.
 */
TEST_F(BasicHexMapTest, GetInvalidVertexRiskTest) {
  BasicHexMap map(planet_1_);

  EXPECT_THROW(map.get_risk(kInvalidHexVertexId), std::runtime_error);
  EXPECT_THROW(map.get_risk(static_cast<HexVertexId>(planet_1_.vertex_count()) + 1), std::runtime_error);
}
