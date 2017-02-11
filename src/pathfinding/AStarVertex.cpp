// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarVertex.h"

AStarVertex::AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost)
    : hex_vertex_id(hex_vertex_id), time(time), cost(cost) {}

bool AStarVertex::operator==(const AStarVertex &other) {
  return hex_vertex_id == other.hex_vertex_id &&
      time == other.time;
}
