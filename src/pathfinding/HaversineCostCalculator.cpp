// Copyright 2017 UBC Sailbot

#include "pathfinding/HaversineCostCalculator.h"

#include <logic/StandardCalc.h>

HaversineCostCalculator::HaversineCostCalculator(HexPlanet &planet) : CostCalculator(planet) {}

CostCalculator::Result HaversineCostCalculator::calculate(HexVertexId source,
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
