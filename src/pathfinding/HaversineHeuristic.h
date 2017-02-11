// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_HAVERSINEHEURISTIC_H_
#define PATHFINDING_HAVERSINEHEURISTIC_H_

#include "Heuristic.h"

class HaversineHeuristic: public Heuristic {
 public:
  HaversineHeuristic();

  /**
   * Computes the a distance heuristic between two points using the Haversine formula.
   * @param source Source coordinate
   * @param target Target coordinate
   * @return Distance in meters
   */
  uint32_t calculate(const GPSCoordinateFast &source, const GPSCoordinateFast &target) override;
};

#endif  // PATHFINDING_HAVERSINEHEURISTIC_H_
