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
   * @param id_time_index Identifier for the vertex and time.
   * @param cost The cost to this vertex from the start.
   */
  AStarVertex(const IdTimeIndex &id_time_index, uint32_t cost)
      : id_time_index_(id_time_index), cost_(cost) {}

  /**
   * @param hex_vertex_id Identifier for the vertex.
   * @param time Time step.
   * @param cost The cost to this vertex from the start.
   */
  AStarVertex(HexVertexId hex_vertex_id, uint32_t time, uint32_t cost)
      : id_time_index_({hex_vertex_id, time}), cost_(cost) {}

  HexVertexId hex_vertex_id() const { return id_time_index_.first; }

  uint32_t time() const { return id_time_index_.second; }

  uint32_t cost() const { return cost_; }

  IdTimeIndex id_time_index() const { return id_time_index_; }

  bool operator<(const AStarVertex &rhs) const {
    return cost_ < rhs.cost();
  }

  bool operator>(const AStarVertex &rhs) const {
    return cost_ > rhs.cost();
  }

 private:
  /// The vertex id and time step for indexing closed set.
  IdTimeIndex id_time_index_;
  /// The cost to this vertex from the start.
  uint32_t cost_;
};

#endif  // PATHFINDING_ASTARVERTEX_H_
