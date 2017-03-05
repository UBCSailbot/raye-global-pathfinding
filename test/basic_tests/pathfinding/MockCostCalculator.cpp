// Copyright 2017 UBC Sailbot

#include "MockCostCalculator.h"

MockCostCalculator::MockCostCalculator(const HexPlanet &planet, const MockCostMap &mock_cost_map, uint32_t default_cost)
    : CostCalculator(planet), mock_cost_map_(mock_cost_map), default_cost_(default_cost) {}

uint32_t MockCostCalculator::calculate(const HexVertexId source, const HexVertexId target, uint32_t time) const {
  if (mock_cost_map_.count(std::make_tuple(source, target, time))) {
    return mock_cost_map_.find(std::make_tuple(source, target, time))->second;
  } else {
    return default_cost_;
  }
}
