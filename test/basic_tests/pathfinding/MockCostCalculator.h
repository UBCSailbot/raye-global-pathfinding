// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_MOCKCOSTCALCULATOR_H_
#define PATHFINDING_MOCKCOSTCALCULATOR_H_

#include <boost/unordered_map.hpp>

#include <pathfinding/CostCalculator.h>

class MockCostCalculator : public CostCalculator {
 public:
  typedef boost::unordered_map<std::tuple<HexVertexId, HexVertexId, uint32_t>, uint32_t> MockCostMap;

  explicit MockCostCalculator(const HexPlanet &planet, const MockCostMap &mock_cost_map, uint32_t default_cost = 1);

  /**
   * @param source Source hex vertex id.
   * @param target Target hex vertex id.
   * @return The cost for an edge.
   */
  uint32_t calculate(const HexVertexId source, const HexVertexId target, uint32_t time) const override;

 private:
  MockCostMap mock_cost_map_;
  uint32_t default_cost_;
};

#endif  //  PATHFINDING_MOCKCOSTCALCULATOR_H_
