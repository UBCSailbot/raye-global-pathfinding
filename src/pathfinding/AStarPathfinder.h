// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARPATHFINDER_H_
#define PATHFINDING_ASTARPATHFINDER_H_

#include "pathfinding/Pathfinder.h"

class AStarPathfinder : public Pathfinder {
 public:
  /**
   * Creates a Pathfinder instance. Each instance pertains to a specific pathfinding scenario.
   * Note: ensure that the heuristic and cost_calculator are compatible!
   * @param planet Planet to use.
   * @param heuristic Heuristic to use.
   * @param cost_calculator CostCalculator to use.
   * @param start Start vertex id.
   * @param target Target vertex id.
   */
  AStarPathfinder(const HexPlanet &planet,
                  Heuristic *heuristic,
                  CostCalculator *cost_calculator,
                  HexVertexId start,
                  HexVertexId target);

  /**
   * Find the path from start to target.
   * @throw std::runtime_error Pathfinding error.
   * @return The path from start_ to target_.
   */
  std::vector<HexVertexId> Run();
};

#endif  // PATHFINDING_ASTARPATHFINDER_H_
