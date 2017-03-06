// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicCostCalculator.h"

#include <iostream>

#include "logic/StandardCalc.h"

BasicCostCalculator::BasicCostCalculator(HexPlanet &planet, std::unique_ptr<BasicHexMap> &map)
    : CostCalculator(planet), map_(std::move(map)) {}

CostCalculator::Result BasicCostCalculator::calculate(HexVertexId target,
                                                      HexVertexId source,
                                                      uint32_t start_time) const {
  // Check that they are valid vertices
  if (target >= planet_.vertex_count() || source >= planet_.vertex_count()) {
    throw std::runtime_error("Target or source vertex does not exist.");
  }

  uint32_t distance_cost = planet_.DistanceBetweenVertices(source, target);
  uint32_t map_cost = map_->get_risk(source) + map_->get_risk(target);
  uint32_t cost = distance_cost + map_cost;

  // TODO(areksredzki): Use better logic for handling time steps.
  uint32_t end_time = start_time + 1;

  return {cost, end_time};
}
