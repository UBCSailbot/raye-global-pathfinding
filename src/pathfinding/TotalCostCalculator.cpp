// Copyright 2020 UBC Sailbot

#include "pathfinding/TotalCostCalculator.h"
#include "pathfinding/HaversineCostCalculator.h"
#include "pathfinding/WeatherCostCalculator.h"

#include <logic/StandardCalc.h>

TotalCostCalculator::TotalCostCalculator(HexPlanet &planet) : CostCalculator(planet) {}

CostCalculator::Result TotalCostCalculator::calculate_neighbour(HexVertexId source,
                                                                    size_t neighbour,
                                                                    uint32_t start_time) const {
  const HexVertex &source_vertex = planet_.vertex(source);
  if (neighbour >= source_vertex.neighbour_count) {
    throw std::runtime_error("Calculating distance to invalid neighbour");
  }

  uint32_t distance = source_vertex.neighbour_distances[neighbour];

  // TODO(areksredzki): Use better logic for handling time steps.
  uint32_t end_time = start_time + 1;

  return {distance, end_time};
}

CostCalculator::Result HaversineCostCalculator::calculate_target(HexVertexId source,
                                                                 HexVertexId target,
                                                                 uint32_t start_time) const {
  // Check that they are valid vertices
  if (target >= planet_.vertex_count() || source >= planet_.vertex_count()) {
    throw std::runtime_error("Target or source vertex does not exist.");
  }

  uint32_t distance = planet_.DistanceBetweenVertices(source, target);

  // TODO(areksredzki): Use better logic for handling time steps.
  uint32_t end_time = start_time + 1;

  return {distance, end_time};
}
