// Copyright 2017 UBC Sailbot

#include "pathfinding/NaiveCostCalculator.h"

NaiveCostCalculator::NaiveCostCalculator(HexPlanet &planet, uint32_t cost) : CostCalculator(planet), cost_(cost) {}

CostCalculator::Result NaiveCostCalculator::calculate(const HexVertexId,
                                                      const HexVertexId,
                                                      uint32_t start_time) const {
  return {cost_, start_time + 1};
}
