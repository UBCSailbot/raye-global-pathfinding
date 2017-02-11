// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_NAIVECOSTCALCULATOR_H_
#define PATHFINDING_NAIVECOSTCALCULATOR_H_

#include "CostCalculator.h"

class NaiveCostCalculator: public CostCalculator {
 public:
  explicit NaiveCostCalculator(const HexPlanet &planet);

  /**
   * @param source Source hex vertex id.
   * @param target Target hex vertex id.
   * @return The cost for an edge.
   */
  uint32_t calculate(HexVertexId target, HexVertexId source, uint32_t time) override;
};

#endif  // PATHFINDING_NAIVECOSTCALCULATOR_H_
