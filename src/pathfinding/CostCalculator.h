// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_COSTCALCULATOR_H_
#define PATHFINDING_COSTCALCULATOR_H_

#include <cstdint>

#include <planet/HexPlanet.h>
#include <datatypes/HexDefs.h>

class CostCalculator {
 public:
  struct Result {
    uint32_t cost;
    uint32_t time;
  };

  explicit CostCalculator(HexPlanet &planet) : planet_(planet) {}

  /**
   * @param source Source hex vertex ID.
   * @param target Target hex vertex ID.
   * @param start_time Starting time step.
   * @return The cost and ending time step for an edge.
   */
  virtual Result calculate(const HexVertexId source, const HexVertexId target, uint32_t start_time) const = 0;

 protected:
  HexPlanet &planet_;
};

#endif  // PATHFINDING_COSTCALCULATOR_H_
