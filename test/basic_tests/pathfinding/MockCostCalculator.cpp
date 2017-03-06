// Copyright 2017 UBC Sailbot

#include "MockCostCalculator.h"

MockCostCalculator::MockCostCalculator(HexPlanet &planet, const MockCostMap &mock_cost_map, uint32_t default_cost)
    : CostCalculator(planet), mock_cost_map_(mock_cost_map), default_cost_(default_cost) {}

CostCalculator::Result MockCostCalculator::calculate(const HexVertexId source,
                                                     const HexVertexId target,
                                                     uint32_t start_time) const {
  uint32_t end_time = start_time + 1;
  // Check if the GPSCoordinate has already been created.
  auto search = mock_cost_map_.find(std::make_tuple(source, target, start_time));
  if (search != mock_cost_map_.end()) {
    return {search->second, end_time};
  } else {
    return {default_cost_, end_time};
  }
}
