// Copyright 2017 UBC Sailbot

#include "HexPlanetTest.h"
#include <planet/HexPlanet.h>

/// The planet subdivision count used for tests
static constexpr int kTestPlanetSize = 7;

/// The planet subdivision count used for GetNeighboursTest
static constexpr int kGetNeighboursTestPlanetSize = 3;

HexPlanetTest::HexPlanetTest() {}

TEST_F(HexPlanetTest, HexPlanetCreationTest) {
  HexPlanet hex_planet = HexPlanet(5);
  EXPECT_EQ(hex_planet.vertex_count(), static_cast<size_t>(2432));
}

/*
 * This is a test of the UpdateVertexNeighbours method, which is
 * ran in the HexPlanet constructor.
 * Check that for every vertex "i" in the HexPlanet,
 * every neighbour of vertex "i" contains "i" as a neighbour.
 * This is done with multiple subdivisions.
 */
TEST_F(HexPlanetTest, UpdateVertexNeighboursTest) {
  // Run the test with sub divisions ranging from 0 to kTestPlanetSize
  for (int subdivisions = 0; subdivisions < kTestPlanetSize; subdivisions++) {
    HexPlanet hex_planet = HexPlanet(subdivisions);

    // Check every vertex in the hexplanet
    for (size_t i = 0; i < hex_planet.vertex_count(); i++) {
      HexVertex current_vertex = hex_planet.vertex(i);

      auto neighbours = current_vertex.neighbours;

      // The number of neighbours of a vertex must be 5 or 6
      EXPECT_TRUE(current_vertex.neighbour_count == 5 || current_vertex.neighbour_count == 6);

      // If there are 5 valid neighbours, check that the last vertex is kInvalidHexVertexId
      if (current_vertex.neighbour_count == 5) {
        EXPECT_EQ(kInvalidHexVertexId, neighbours.at(5));
      }

      // Check every neighbour of vertex i, and make sure it contains i as a neighbour
      for (HexVertexId j = 0; j < current_vertex.neighbour_count; j++) {
        HexVertexId neighbour_id = neighbours.at(j);

        auto neighbours_of_neighbour = hex_planet.vertex(neighbour_id).neighbours;

        // We expect vertex i to be a neighbour, since this vertex (vertex "id") is one of neighbours of i.
        EXPECT_TRUE(std::find(neighbours_of_neighbour.begin(), neighbours_of_neighbour.end(), i) !=
            neighbours_of_neighbour.end());
      }
    }
  }
}

// Test that neighbour reciprocity using HexPlanet methods
// Also test that vertices have the expected number of neighbours
TEST_F(HexPlanetTest, GetNeighboursTest) {
  HexPlanet planet = HexPlanet(kGetNeighboursTestPlanetSize);
  size_t num_vertices = planet.vertex_count();

  for (HexVertexId i = 0; i < num_vertices; ++i) {
    std::array<HexVertexId, 6> neighbours;
    planet.GetNeighbours(i, &neighbours);
    bool has_invalid_vertex = false;

    for (const HexVertexId &neighbour_id : neighbours) {
      // there should be a maximum of one invalid vertex per neighbour
      if (neighbour_id == kInvalidHexVertexId) {
        EXPECT_FALSE(has_invalid_vertex);
        has_invalid_vertex = true;
      } else {
        // check for neighbourship reciprocity
        std::array<HexVertexId, 6> next_neighbours;
        planet.GetNeighbours(neighbour_id, &next_neighbours);

        EXPECT_TRUE(std::find(next_neighbours.begin(), next_neighbours.end(), i)
                        != next_neighbours.end());
      }
    }
  }
}
