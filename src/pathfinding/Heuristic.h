// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_HEURISTIC_H_
#define PATHFINDING_HEURISTIC_H_

#include <cstdint>

#include "planet/HexPlanet.h"
#include "datatypes/GPSCoordinateFast.h"

/**
 * Heuristics provide a way of measuring distance between points.
 *
 * Note: an admissable heuristic must always be a lower bound on the cost calculator being used.
 */
class Heuristic {
 public:
  explicit Heuristic(HexPlanet &planet) : planet_(planet) {}

  /**
   * @param source Source vertex id
   * @param target Target vertex id
   * @return A heuristic costs
   */
  virtual uint32_t calculate(HexVertexId source, HexVertexId target) const = 0;

 protected:
  HexPlanet &planet_;
};

#endif  // PATHFINDING_HEURISTIC_H_
