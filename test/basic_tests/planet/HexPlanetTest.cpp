// Copyright 2017 UBC Sailbot

#include "HexPlanetTest.h"

#include <planet/HexPlanet.h>

/// The planet subdivision count used for tests
static constexpr uint8_t kTestPlanetSize = 6;
static constexpr size_t kTestPlanetVertexCount = 7292;
static constexpr size_t kTestPlanetTriangleCount = 14580;

HexPlanetTest::HexPlanetTest() {}

TEST_F(HexPlanetTest, HexPlanetCreationTest) {
  HexPlanet hex_planet = HexPlanet(kTestPlanetSize, 0);
  EXPECT_EQ(hex_planet.vertex_count(), kTestPlanetVertexCount);
  EXPECT_EQ(hex_planet.triangle_count(), kTestPlanetTriangleCount);
}

/**
 * This is a test of the ComputeVertexNeighbours method, which is run in the HexPlanet constructor.
 * Check that for every vertex "i" in the HexPlanet, every neighbour of vertex "i" contains "i" as a neighbour.
 * This is done with multiple subdivisions.
 */
TEST_F(HexPlanetTest, ComputeVertexNeighboursTest) {
  // Run the test with sub divisions ranging from 0 to kTestPlanetSize
  for (uint8_t subdivisions = 0; subdivisions <= kTestPlanetSize; subdivisions++) {
    HexPlanet hex_planet = HexPlanet(subdivisions, 0);

    // Check every vertex in the HexPlanet
    for (HexVertexId i = 0; i < hex_planet.vertex_count(); i++) {
      HexVertex current_vertex = hex_planet.vertex(i);

      auto &neighbours = current_vertex.neighbours;

      // The number of neighbours of a vertex must be 5 or 6
      EXPECT_TRUE(current_vertex.neighbour_count == 5 || current_vertex.neighbour_count == 6);

      // If there are 5 valid neighbours, check that the last vertex is kInvalidHexVertexId
      if (current_vertex.neighbour_count == 5) {
        EXPECT_EQ(kInvalidHexVertexId, neighbours.at(5));
      }

      // Check every neighbour of vertex i, and make sure it contains i as a neighbour
      for (HexVertexId j = 0; j < current_vertex.neighbour_count; j++) {
        HexVertexId neighbour_id = neighbours.at(j);

        const auto &neighbours_of_neighbour = hex_planet.vertex(neighbour_id).neighbours;

        // We expect vertex i to be a neighbour, since this vertex (vertex "id") is one of neighbours of i.
        EXPECT_TRUE(std::find(neighbours_of_neighbour.begin(), neighbours_of_neighbour.end(), i) !=
            neighbours_of_neighbour.end());
      }
    }
  }
}

/**
 * This is a test of the ComputeIndirectVertexNeighbours method, which is run in the HexPlanet constructor.
 * Check that for every vertex "i" in the HexPlanet, every neighbour of vertex "i" contains "i" as a neighbour.
 * This is done with multiple subdivisions.
 */
TEST_F(HexPlanetTest, ComputeIndirectVertexNeighboursTest) {
  static constexpr uint8_t kTestMaxPlanetSize = 3;
  static constexpr uint8_t kTestMaxIndirectNeighbourDepth = 3;

  // Run the test with sub divisions ranging from 0 to kTestMaxPlanetSize
  for (uint8_t subdivisions = 0; subdivisions <= kTestMaxPlanetSize; subdivisions++) {
    // Check that no indirect neighbours are created for a indirect_neighbour_depth of 0
    HexPlanet hex_planet = HexPlanet(subdivisions, 0);
    for (HexVertexId i = 0; i < hex_planet.vertex_count(); i++) {
      HexVertex current_vertex = hex_planet.vertex(i);
      EXPECT_EQ(0u, current_vertex.indirect_neighbours.size());
    }

    // Check all indirect_neighbour_depths from 1 to kTestMaxIndirectNeighbourDepth
    for (uint8_t indirect_neighbour_depth = 1; indirect_neighbour_depth <= kTestMaxIndirectNeighbourDepth;
         indirect_neighbour_depth++) {
      hex_planet = HexPlanet(subdivisions, indirect_neighbour_depth);

      // Check every vertex in the HexPlanet
      for (HexVertexId i = 0; i < hex_planet.vertex_count(); i++) {
        HexVertex current_vertex = hex_planet.vertex(i);

        const auto &indirect_neighbours = current_vertex.indirect_neighbours;

        // The number of indirect neighbours of a vertex in a HexPlanet with a positive indirect_neighbour_depth will
        // always be greater than 4
        EXPECT_GT(indirect_neighbours.size(), 4u);

        // Check that the current node isn't in indirect_neighbours.
        EXPECT_EQ(indirect_neighbours.end(), std::find(indirect_neighbours.begin(), indirect_neighbours.end(), i));

        // Check that none of the direct neighbours exist in indirect_neighbours.
        for (HexVertexId j = 0; j < current_vertex.neighbour_count; j++) {
          HexVertexId neighbour_id = current_vertex.neighbours.at(j);
          EXPECT_EQ(indirect_neighbours.end(),
                    std::find(indirect_neighbours.begin(), indirect_neighbours.end(), neighbour_id));
        }
      }
    }
  }
}
