// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicCostCalculator.h"

#include <iostream>

BasicCostCalculator::BasicCostCalculator(HexPlanet &planet, std::unique_ptr<BasicHexMap> &map)
    : HaversineCostCalculator(planet), map_(std::move(map)) {}

CostCalculator::Result BasicCostCalculator::calculate_neighbour(HexVertexId source,
                                                                size_t neighbour,
                                                                uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_neighbour(source, neighbour, start_time);

  // Get neighbour vertex ID; |neighbour| is valid because otherwise an exception would have been thrown earlier.
  HexVertexId target = planet_.vertex(source).neighbours[neighbour];

  result.cost += calculate_map_cost(source, target, start_time);

  return result;
}

CostCalculator::Result BasicCostCalculator::calculate_target(HexVertexId source,
                                                             HexVertexId target,
                                                             uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_target(source, target, start_time);

  result.cost += calculate_map_cost(source, target, start_time);

  return result;
}

uint32_t BasicCostCalculator::calculate_map_cost(HexVertexId target, HexVertexId source, uint32_t) const {
  return map_->get_risk(source) + map_->get_risk(target);
}
