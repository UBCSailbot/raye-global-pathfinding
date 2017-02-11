// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_ASTARVERTEX_H_
#define PATHFINDING_ASTARVERTEX_H_

#include <cstdint>

#include "datatypes/HexDefs.h"

/**
 * @brief This is used by the AStar Pathfinder to store state expansions.
 */
class AStarVertex {
 public:
  /// Identifier for the vertex
  const HexVertexId hex_vertex_id;
  /// Time step TODO(areksredzki): better define the units used here
  const uint32_t time;
  /// The cost to this vertex + the heuristic to to the goal
  const double cost;

  /**
   * @param hex_vertex_id Identifier for the vertex
   * @param time Time step TODO(areksredzki): better define the units used here
   * @param cost The cost to this vertex + the heuristic to to the goal
   */
  AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost);

  /**
   * @param other The vertex to compare against.
   * @return Whether the two instances have the same position and time (NOT COST).
   */
  bool operator==(const AStarVertex &other);
};

#endif  // PATHFINDING_ASTARVERTEX_H_
