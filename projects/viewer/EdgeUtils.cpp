// Copyright 2017 UBC Sailbot

#include "EdgeUtils.h"

#include <random>
#include <pathfinding/CostCalculator.h>

/**
 * Generates a mapping of random costs for each edge.
 * Helper function. Currently for testing, might be useful in the future.
 */
EdgeRenderer::EdgeCostMap EdgeUtils::GenerateRandomEdgeCostMap(const VertexPairVector &edge_vertex_pairs) {
  // Random number generation setup.
  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<> dist(0.0f, 1.0f);

  EdgeRenderer::EdgeCostMap edges;

  for (const auto &vertex_pair : edge_vertex_pairs) {
    edges[vertex_pair] = static_cast<float>(dist(e2));
  }

  return edges;
}

EdgeRenderer::EdgeCostMap EdgeUtils::GenerateEdgeCostMap(const VertexPairVector &edge_vertex_pairs,
                                                         const CostCalculator &cost_calculator,
                                                         uint32_t time_step) {
  EdgeRenderer::EdgeCostMap edges;

  float min = std::numeric_limits<float>().max();
  float max = std::numeric_limits<float>().min();

  for (const auto &vertex_pair : edge_vertex_pairs) {
    auto result = cost_calculator.calculate_target(vertex_pair.first, vertex_pair.second, time_step);
    float cost = result.cost;
    min = std::min(min, cost);
    max = std::max(max, cost);
    edges[vertex_pair] = cost;
  }

  // Handle the case where there is no difference between min & max.
  if (min == max) {
    for (auto &edge : edges) {
      edge.second = 0.5f;
    }
  } else {
    // Normalize between 0 and 1 based on the min and max seen.
    for (auto &edge : edges) {
      edge.second = (edge.second - min) / (max - min);
    }
  }

  return edges;
}

EdgeRenderer::EdgeCostMap EdgeUtils::GenerateRandomPlanetEdgeCostMap(const std::unique_ptr<HexPlanet> &planet) {
  return GenerateRandomEdgeCostMap(GeneratePlanetEdgeVertexPairs(planet));
}

EdgeRenderer::EdgeCostMap EdgeUtils::GeneratePlanetEdgeCostMap(const std::unique_ptr<HexPlanet> &planet,
                                                               const CostCalculator &cost_calculator,
                                                               uint32_t time_step) {
  return GenerateEdgeCostMap(GeneratePlanetEdgeVertexPairs(planet), cost_calculator, time_step);
}

EdgeRenderer::EdgeCostMap EdgeUtils::GeneratePathEdgeCostMap(const std::vector<HexVertexId> &path,
                                                             const CostCalculator &cost_calculator,
                                                             uint32_t time_step) {
  return GenerateEdgeCostMap(GeneratePathVertexPairs(path), cost_calculator, time_step);
}

EdgeUtils::VertexPairVector EdgeUtils::GeneratePlanetEdgeVertexPairs(const std::unique_ptr<HexPlanet> &planet) {
  VertexPairVector edge_vertex_pairs;

  for (size_t i = 0; i < planet->triangle_count(); ++i) {
    const HexTriangle &triangle = planet->triangle(i);

    edge_vertex_pairs.push_back({triangle.vertex_a, triangle.vertex_b});
    edge_vertex_pairs.push_back({triangle.vertex_b, triangle.vertex_c});
    edge_vertex_pairs.push_back({triangle.vertex_c, triangle.vertex_a});
  }

  return edge_vertex_pairs;
}

EdgeUtils::VertexPairVector EdgeUtils::GeneratePathVertexPairs(const std::vector<HexVertexId> &path) {
  EdgeUtils::VertexPairVector path_edges;

  for (size_t i = 1; i < path.size(); i++) {
    path_edges.push_back({path.at(i - 1), path.at(i)});
  }

  return path_edges;
}
