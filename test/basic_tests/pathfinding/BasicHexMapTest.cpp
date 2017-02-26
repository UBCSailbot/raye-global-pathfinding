// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicHexMapTest.h"

/// Size of planet used in BasicHexMapTests
static constexpr size_t kSizeOfTestPlanet = 4;

BasicHexMapTest::BasicHexMapTest()
    : planet(HexPlanet(kSizeOfTestPlanet)),
      map(BasicHexMap(planet)) { }

/**
 * Test that the risk for all the vertices are within a valid range.
 */
TEST_F(BasicHexMapTest, VertexRisksWithinBoundsTest) {
  for (HexVertexId i = 0; i < planet.vertex_count(); ++i) {
    uint32_t vertex_risk = map.get_risk(i);
    EXPECT_LE(vertex_risk, BasicHexMap::kMaxVertexRisk);
    EXPECT_GE(vertex_risk, 1);
  }
}

/**
 * Test that an invalid risk value is correctly returned for vertex IDs that
 * do not exist on the planet.
 */
TEST_F(BasicHexMapTest, GetInvalidVertexRiskTest) {
  EXPECT_THROW(map.get_risk(kInvalidHexVertexId), std::runtime_error);
  EXPECT_THROW(map.get_risk(static_cast<HexVertexId>(planet.vertex_count()) + 1),
               std::runtime_error);
}
