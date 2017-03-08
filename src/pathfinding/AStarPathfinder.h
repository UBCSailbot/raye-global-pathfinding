// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARPATHFINDER_H_
#define PATHFINDING_ASTARPATHFINDER_H_

#include <boost/unordered_map.hpp>

#include "pathfinding/Pathfinder.h"
#include "pathfinding/AStarVertex.h"

class AStarPathfinder : public Pathfinder {
 public:
  /// The number of states for which to reserve the closed set for to avoid rehashing.
  static constexpr size_t kClosedSetReserveSize = 5000000;
  /// The minimum planet subdivision number for the closed set reservation to be active.
  static constexpr int kClosedSetReservePlanetSize = 9;

  /**
   * Creates a Pathfinder instance. Each instance pertains to a specific pathfinding scenario.
   * Note: ensure that the heuristic and cost_calculator are compatible!
   * @param planet Planet to use.
   * @param heuristic Heuristic to use.
   * @param cost_calculator CostCalculator to use.
   * @param start Start vertex id.
   * @param target Target vertex id.
   */
  AStarPathfinder(HexPlanet &planet,
                  const Heuristic &heuristic,
                  const CostCalculator &cost_calculator,
                  HexVertexId start,
                  HexVertexId target);

  /**
   * Find the path from start to target.
   * @throw std::runtime_error Pathfinding error.
   * @return The path from start_ to target_.
   */
  Result Run();

 private:
  typedef boost::unordered_map<AStarVertex::IdTimeIndex, AStarVertex> TimeIndexVertexMap;

  std::vector<HexVertexId> ConstructPath(AStarVertex vertex, const TimeIndexVertexMap &closed_set);
};

#endif  // PATHFINDING_ASTARPATHFINDER_H_
