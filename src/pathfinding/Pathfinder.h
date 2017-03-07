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
   * The result of a pathfinder run.
   */
  struct Result {
    std::vector<HexVertexId> path;
    double cost;
    uint32_t time;
  };

  /**
   * Object for storing stats that would normally be lost outside of the scope of Run().
   */
  struct Stats {
    /// The size of the closed set at the end of Run(). This is the number of states (vertex & time) visited.
    size_t closed_set_size;
    /// The size of the open set at the end of Run(). This is the number of states (vertex & time) to be visited.
    size_t open_set_size;
  };

  /**
   * Creates a Pathfinder instance. Each instance pertains to a specific pathfinding scenario.
   * Note: ensure that the heuristic and cost_calculator are compatible!
   * @param planet Planet to use.
   * @param heuristic Heuristic to use.
   * @param cost_calculator CostCalculator to use.
   * @param start Start vertex id.
   * @param target Target vertex id.
   */
  Pathfinder(HexPlanet &planet,
             const Heuristic &heuristic,
             const CostCalculator &cost_calculator,
             HexVertexId start,
             HexVertexId target);

  /**
   * Find the path from start to target.
   * @throw std::runtime_error Pathfinding error.
   * @return The path from start_ to target_.
   */
  virtual Result Run() = 0;

  /**
   * @return The stats computed during the pathfinder run.
   */
  const Stats &stats() const;

 protected:
  HexPlanet &planet_;
  const Heuristic &heuristic_;
  const CostCalculator &cost_calculator_;

  const HexVertexId start_;
  const HexVertexId target_;

  Stats stats_ = {0, 0};
};

#endif  // PATHFINDING_PATHFINDER_H_
