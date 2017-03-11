// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARPATHFINDER_H_
#define PATHFINDING_ASTARPATHFINDER_H_

#include <boost/unordered_map.hpp>
#include <queue>

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
  struct VisitedStateData {
    /// The cost to this vertex (and time) from the start.
    uint32_t cost;
    /// The ancestor to this node.
    AStarVertex::IdTimeIndex parent;
  };

  typedef std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<AStarVertex>> VertexQueue;
  typedef boost::unordered_map<AStarVertex::IdTimeIndex, VisitedStateData> TimeIndexValueMap;

  /**
   * If a neighbour state expansion provides a new lowest cost to the neighbour, add it to the open set and visited
   * state data.
   * @param open_set The open set.
   * @param visited Visited state data.
   * @param current_id_time_index IdTimeIndex of the "current" state.
   * @param neighbour_id_time_index IdTimeIndex of the "neighbour" state.
   * @param neighbour_cost The cost from start to the neighbour state. Note: this is not just cost from "current".
   * @param heuristic_cost The heuristic cost to the target.
   */
  void AddNeighbour(VertexQueue &open_set,
                    TimeIndexValueMap &visited,
                    const AStarVertex::IdTimeIndex &current_id_time_index,
                    const AStarVertex::IdTimeIndex &neighbour_id_time_index,
                    uint32_t neighbour_cost,
                    uint32_t heuristic_cost);

  std::vector<HexVertexId> ConstructPath(AStarVertex::IdTimeIndex vertex, const TimeIndexValueMap &visited);
};

#endif  // PATHFINDING_ASTARPATHFINDER_H_
