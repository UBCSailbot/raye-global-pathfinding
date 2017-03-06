// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_HAVERSINEHEURISTIC_H_
#define PATHFINDING_HAVERSINEHEURISTIC_H_

#include "pathfinding/Heuristic.h"

class HaversineHeuristic: public Heuristic {
 public:
  explicit HaversineHeuristic(HexPlanet &planet);

  /**
   * Computes the a distance heuristic between two points using the Haversine formula.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @return Distance in meters
   */
  uint32_t calculate(HexVertexId source, HexVertexId target) const override;
};

#endif  // PATHFINDING_HAVERSINEHEURISTIC_H_
