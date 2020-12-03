// Copyright 2020 UBC Sailbot

#ifndef PATHFINDING_TOTALCOSTCALCULATOR_H_
#define PATHFINDING_TOTALCOSTCALCULATOR_H_

#include "pathfinding/CostCalculator.h"

class TotalCostCalculator : public CostCalculator {
 public:
  explicit TotalCostCalculator(HexPlanet &planet);

  /**
   * Calculate the cost to an immediate neighbour of |source| using the Haversine formula.
   * @param source Source hex vertex ID.
   * @param neighbour Target hex vertex's position in |source|'s neighbour array.
   * @param start_time Starting time step.
   * @throw std::runtime_error |neighbour| is invalid.
   * @return The cost and ending time step for an edge.
   */
  Result calculate_neighbour(HexVertexId source, size_t neighbour, uint32_t start_time) const override;

  /**
   * Computes the a distance between two points using the Haversine formula.
   * Note: Currently just increments time by one.
   * @warning Use calculate_neighbour() if possible!
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param time Starting time step.
   * @throw std::runtime_error If target or source does not exist on the planet.
   * @return The cost (distance in meters) and ending time step for an edge.
   */
  Result calculate_target(HexVertexId source, HexVertexId target, uint32_t start_time) const override;

  /**
   * @return Whether this cost calculator is safe for usage with indirect neighbours.
   */
  bool is_indirect_neighbour_safe() const override { return true; }
};

#endif  // PATHFINDING_TOTALCOSTCALCULATOR_H_
