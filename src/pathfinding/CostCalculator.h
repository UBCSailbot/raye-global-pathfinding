// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_COSTCALCULATOR_H_
#define PATHFINDING_COSTCALCULATOR_H_

#include <cstdint>

#include <planet/HexPlanet.h>
#include <datatypes/HexDefs.h>

class CostCalculator {
 public:
  explicit CostCalculator(const HexPlanet &planet) : planet_(planet) {}

  /**
   * @param source Source hex vertex id.
   * @param target Target hex vertex id.
   * @return The cost for an edge.
   */
  virtual uint32_t calculate(const HexVertexId source, const HexVertexId target, uint32_t time) const = 0;

 private:
  const HexPlanet &planet_;
};

#endif  // PATHFINDING_COSTCALCULATOR_H_
