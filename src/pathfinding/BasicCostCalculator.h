// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICCOSTCALCULATOR_H_
#define PATHFINDING_BASICCOSTCALCULATOR_H_

#include "pathfinding/BasicHexMap.h"
#include "pathfinding/CostCalculator.h"
#include "planet/HexPlanet.h"

class BasicCostCalculator: public CostCalculator {
 public:
  /**
   * Creates a BasicCostCalculator instance that gets the cost from one point
   * to another. The Calculator will take ownership of the map it is given and
   * handle its deletion.
   * @param planet The planet.
   * @param map The risk map for the planet.
   * @throw std::runtime_error If map pointer is null.
   */
  explicit BasicCostCalculator(const HexPlanet &planet, BasicHexMap *map);

  /**
   * Calculates the cost to get between two points.
   * @param target The destination vertex ID.
   * @param source The source vertex ID.
   * @throw std::runtime_error if target or source does not exist on the planet.
   * @return The cost or risk to get from source to target.
   */
  uint32_t calculate(const HexVertexId target, const HexVertexId source, uint32_t) const override;

  /**
   * Destroy this cost calculator and its associated risk map.
   */
  ~BasicCostCalculator();

  // Class can't be copied
  BasicCostCalculator(const BasicCostCalculator&) = delete;

  // Class can't be moved
  BasicCostCalculator(BasicCostCalculator&&) = delete;

  // Class can't be copy assigned
  BasicCostCalculator &operator= (const BasicCostCalculator&) = delete;

  // Class can't be move assigned
  BasicCostCalculator &operator=(BasicCostCalculator&&) = delete;

 private:
  HexPlanet planet_;
  BasicHexMap *map_;
};

#endif  // PATHFINDING_BASICCOSTCALCULATOR_H_
