// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicCostCalculator.h"
#include <iostream>

BasicCostCalculator::BasicCostCalculator(const HexPlanet &planet,
                                         BasicHexMap *map)
    : CostCalculator(planet),
      planet_(planet),
      map_(map) {
  if (map == nullptr) {
    throw std::runtime_error("BasicHexMap must not be null.");
  }
}

uint32_t BasicCostCalculator::calculate(
        const HexVertexId target,
        const HexVertexId source,
        uint32_t) const {
  // check that they are valid vertices
  if (target > planet_.vertex_count() || source > planet_.vertex_count()) {
    throw std::runtime_error("Target or source vertex does not exist.");
  }

  return map_->get_risk(source) + map_->get_risk(target);
}

BasicCostCalculator::~BasicCostCalculator() {
  delete map_;
}
