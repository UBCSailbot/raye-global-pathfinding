// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_MOCKCOSTCALCULATOR_H_
#define PATHFINDING_MOCKCOSTCALCULATOR_H_

#include <boost/unordered_map.hpp>

#include <pathfinding/CostCalculator.h>

class MockCostCalculator : public CostCalculator {
 public:
  typedef boost::unordered_map<std::tuple<HexVertexId, HexVertexId, uint32_t>, uint32_t> MockCostMap;

  explicit MockCostCalculator(HexPlanet &planet, const MockCostMap &mock_cost_map, uint32_t default_cost = 1);

  /**
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param start_time Starting time step.
   * @return The cost for an edge.
   */
  Result calculate(const HexVertexId source, const HexVertexId target, uint32_t start_time) const override;

 private:
  MockCostMap mock_cost_map_;
  uint32_t default_cost_;
};

#endif  //  PATHFINDING_MOCKCOSTCALCULATOR_H_
