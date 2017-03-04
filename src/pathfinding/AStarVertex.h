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
  typedef std::pair<HexVertexId, uint32_t> IdTimeCoordinate;

  /**
   * @param hex_vertex_id Identifier for the vertex
   * @param time Time step TODO(areksredzki): better define the units used here
   * @param cost The cost to this vertex + the heuristic to to the goal
   */
  AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost);

  /**
   * @param hex_vertex_id Identifier for the vertex
   * @param time Time step TODO(areksredzki): better define the units used here
   * @param cost The cost to this vertex + the heuristic to to the goal
   * @param parent The ancestor to this node
   */
  AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost, IdTimeCoordinate parent);

  HexVertexId hex_vertex_id() const;

  uint32_t time() const;

  double cost() const;

  IdTimeCoordinate parent() const;

  IdTimeCoordinate id_time_coordinate() const;

  void set_parent(const IdTimeCoordinate &parent);

  bool operator<(const AStarVertex &rhs) const;

 private:
  /// Identifier for the vertex
  HexVertexId hex_vertex_id_;
  /// Time step TODO(areksredzki): better define the units used here
  uint32_t time_;
  /// The cost to this vertex + the heuristic to to the goal
  double cost_;
  /// The ancestor to this node
  IdTimeCoordinate parent_;
};

#endif  // PATHFINDING_ASTARVERTEX_H_
