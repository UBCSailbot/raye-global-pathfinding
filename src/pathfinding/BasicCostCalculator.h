// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICCOSTCALCULATOR_H_
#define PATHFINDING_BASICCOSTCALCULATOR_H_

#include "pathfinding/BasicHexMap.h"
#include "pathfinding/HaversineCostCalculator.h"

class BasicCostCalculator : public HaversineCostCalculator {
 public:
  /**
   * Creates a BasicCostCalculator instance that gets the cost from one point
   * to another. The Calculator will take ownership of the map it is given and
   * handle its deletion.
   * @param planet The planet.
   * @param map The risk map for the planet.
   */
  explicit BasicCostCalculator(HexPlanet &planet, std::unique_ptr<BasicHexMap> &map);

  /**
   * Calculate the cost to an immediate neighbour of |source| using the Haversine formula and the BasicHexMap.
   * @param source Source hex vertex ID.
   * @param neighbour Target hex vertex's position in |source|'s neighbour array.
   * @param start_time Starting time step.
   * @throw std::runtime_error |neighbour| is invalid.
   * @return The cost (distance in meters + BasicHexMap based cost) and ending time step for an edge.
   */
  Result calculate_neighbour(HexVertexId source, size_t neighbour, uint32_t start_time) const override;

  /**
   * Computes the a cost between two points using the Haversine formula and the BasicHexMap.
   * Note: Currently just increments time by one.
   * @warning Use calculate_neighbour() if possible!
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param time Starting time step.
   * @throw std::runtime_error If target or source does not exist on the planet.
   * @return The cost (distance in meters + BasicHexMap based cost) and ending time step for an edge.
   */
  Result calculate_target(HexVertexId source, HexVertexId target, uint32_t start_time) const override;

  /// Class can't be copied
  BasicCostCalculator(const BasicCostCalculator &) = delete;

  /// Class can't be moved
  BasicCostCalculator(BasicCostCalculator &&) = delete;

  /// Class can't be copy assigned
  BasicCostCalculator &operator=(const BasicCostCalculator &) = delete;

  /// Class can't be move assigned
  BasicCostCalculator &operator=(BasicCostCalculator &&) = delete;

  /**
   * @return Whether this cost calculator is safe for usage with indirect neighbours.
   */
  bool is_indirect_neighbour_safe() const override { return false; }

 private:
  std::unique_ptr<BasicHexMap> map_;
  uint32_t calculate_map_cost(HexVertexId target, HexVertexId source, uint32_t start_time) const;
};

#endif  // PATHFINDING_BASICCOSTCALCULATOR_H_
