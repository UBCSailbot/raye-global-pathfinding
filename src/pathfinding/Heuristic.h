// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_HEURISTIC_H_
#define PATHFINDING_HEURISTIC_H_

#include <cstdint>

#include "datatypes/GPSCoordinateFast.h"

/**
 * Heuristics provide a way of measuring distance between points.
 *
 * Note: an admissable heuristic must always be a lower bound on the cost calculator being used.
 */
class Heuristic {
 public:
  /**
   * @param source Source coordinate
   * @param target Target coordinate
   * @return A heuristic costs
   */
  virtual uint32_t calculate(const GPSCoordinateFast &source, const GPSCoordinateFast &target) = 0;
};

#endif  // PATHFINDING_HEURISTIC_H_
