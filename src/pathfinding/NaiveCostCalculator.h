// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_NAIVECOSTCALCULATOR_H_
#define PATHFINDING_NAIVECOSTCALCULATOR_H_

#include "CostCalculator.h"

class NaiveCostCalculator: public CostCalculator {
 public:
  /**
   * @param planet The planet used for cost calculations. Note: Not used by this cost calculator.
   * @param cost The cost that will always be returned.
   */
  explicit NaiveCostCalculator(HexPlanet &planet, uint32_t cost = 1);

  /**
   * Note: This cost calculator doesn't actually use the source & target IDs.
   * Note: Currently just increments time by one.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param start_time Starting time step.
   * @return The cost (defined during construction) and ending time step for an edge.
   */
  Result calculate_target(HexVertexId source, HexVertexId target, uint32_t start_time) const override;

 private:
  uint32_t cost_;
};

#endif  // PATHFINDING_NAIVECOSTCALCULATOR_H_
