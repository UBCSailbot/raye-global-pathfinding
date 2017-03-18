// Copyright 2017 UBC Sailbot

#ifndef EDGEUTILS_H_
#define EDGEUTILS_H_

#include <pathfinding/CostCalculator.h>
#include "EdgeRenderer.h"

class EdgeUtils {
 public:
  typedef std::vector<HexPlanet::HexVertexPair> VertexPairVector;

  /**
   * Generates a mapping of costs for each edge using a cost calculator.
   */
  static EdgeRenderer::EdgeCostMap GenerateEdgeCostMap(const VertexPairVector &edge_vertex_pairs,
                                                       const CostCalculator &cost_calculator,
                                                       uint32_t time_step);

  /**
   * Generates a mapping of random costs for each edge.
   */
  static EdgeRenderer::EdgeCostMap GenerateRandomEdgeCostMap(const VertexPairVector &edge_vertex_pairs);

  /**
   * Generates a mapping of random costs for each edge of a planet.
   */
  static EdgeRenderer::EdgeCostMap GenerateRandomPlanetEdgeCostMap(const std::unique_ptr<HexPlanet> &planet);

  /**
   * Generates a mapping of costs for each edge of a planet using a cost calculator.
   */
  static EdgeRenderer::EdgeCostMap GeneratePlanetEdgeCostMap(const std::unique_ptr<HexPlanet> &planet,
                                                             const CostCalculator &cost_calculator,
                                                             uint32_t time_step);

  /**
   * Generates a mapping of costs for each edge of a path using a cost calculator.
   */
  static EdgeRenderer::EdgeCostMap GeneratePathEdgeCostMap(const std::vector<HexVertexId> &path,
                                                           const CostCalculator &cost_calculator,
                                                           uint32_t time_step);

 private:
  /**
   * Generate a VertexPair for each edge in the planet geometry.
   */
  static VertexPairVector GeneratePlanetEdgeVertexPairs(const std::unique_ptr<HexPlanet> &planet);

  /**
   * Generate a VertexPair for each component of the path.
   */
  static VertexPairVector GeneratePathVertexPairs(const std::vector<HexVertexId> &path);
};

#endif  // EDGEUTILS_H_
