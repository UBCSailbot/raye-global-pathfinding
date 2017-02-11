// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_PATHFINDER_H_
#define PATHFINDING_PATHFINDER_H_

#include <vector>

#include "planet/HexPlanet.h"
#include "pathfinding/Heuristic.h"
#include "pathfinding/CostCalculator.h"

class Pathfinder {
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
  Pathfinder(const HexPlanet &planet,
             Heuristic *heuristic,
             CostCalculator *cost_calculator,
             HexVertexId start,
             HexVertexId target);

  /**
   * Find the path from start to target.
   * @throw std::runtime_error Pathfinding error.
   * @return The path from start_ to target_.
   */
  virtual std::vector<HexVertexId> Run() = 0;

 protected:
  const HexPlanet &planet_;
  const Heuristic *heuristic_;
  const CostCalculator *cost_calculator_;

  const HexVertexId start_;
  const HexVertexId target_;
};

#endif  // PATHFINDING_PATHFINDER_H_
