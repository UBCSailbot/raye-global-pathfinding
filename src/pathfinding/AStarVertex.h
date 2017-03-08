// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARVERTEX_H_
#define PATHFINDING_ASTARVERTEX_H_

#include <cstdint>
#include <utility>

#include "datatypes/HexDefs.h"

/**
 * @brief This is used by the AStar Pathfinder to store state expansions.
 */
class AStarVertex {
 public:
  typedef std::pair<HexVertexId, uint32_t> IdTimeIndex;

  /**
   * @param hex_vertex_id Identifier for the vertex.
   * @param time Time step.
   * @param cost The cost to this vertex from the start.
   * @param heuristic_cost The heuristic cost from this vertex to the goal.
   * @param parent The ancestor to this node.
   */
  AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost, double heuristic_cost, IdTimeIndex parent);

  HexVertexId hex_vertex_id() const { return id_time_index_.first; }

  uint32_t time() const { return id_time_index_.second; }

  double cost() const { return cost_; }

  IdTimeIndex parent() const { return parent_; }

  IdTimeIndex id_time_index() const { return id_time_index_; }

  double heuristic_cost() const { return heuristic_cost_; }

  bool operator<(const AStarVertex &rhs) const {
    return cost_ + heuristic_cost_ > rhs.cost() + rhs.heuristic_cost();
  }

 private:
  /// The vertex id and time step for indexing closed set.
  IdTimeIndex id_time_index_;
  /// The cost to this vertex from the start.
  double cost_;
  /// The heuristic cost from this vertex to the goal.
  double heuristic_cost_;
  /// The ancestor to this node.
  IdTimeIndex parent_;
};

#endif  // PATHFINDING_ASTARVERTEX_H_
