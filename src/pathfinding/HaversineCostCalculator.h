// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_HAVERSINECOSTCALCULATOR_H_
#define PATHFINDING_HAVERSINECOSTCALCULATOR_H_

#include "pathfinding/CostCalculator.h"

class HaversineCostCalculator: public CostCalculator {
 public:
  explicit HaversineCostCalculator(HexPlanet &planet);

  /**
   * Computes the a distance between two points using the Haversine formula.
   * Note: Currently just increments time by one.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param time Starting time step.
   * @throw std::runtime_error if target or source does not exist on the planet.
   * @return The cost (distance in meters) and ending time step for an edge.
   */
  Result calculate(const HexVertexId source, const HexVertexId target, uint32_t start_time) const override;
};

#endif  // PATHFINDING_HAVERSINECOSTCALCULATOR_H_
