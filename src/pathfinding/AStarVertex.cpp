// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarVertex.h"

AStarVertex::AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost)
    : hex_vertex_id_(hex_vertex_id), time_(time), cost_(cost), parent_({kInvalidHexVertexId, 0}) {}

AStarVertex::AStarVertex(HexVertexId hex_vertex_id, uint32_t time, double cost, IdTimeCoordinate parent)
    : hex_vertex_id_(hex_vertex_id), time_(time), cost_(cost), parent_(parent) {}

bool AStarVertex::operator<(const AStarVertex &rhs) const {
  return cost_ > rhs.cost();
}
HexVertexId AStarVertex::hex_vertex_id() const {
  return hex_vertex_id_;
}
uint32_t AStarVertex::time() const {
  return time_;
}
double AStarVertex::cost() const {
  return cost_;
}
AStarVertex::IdTimeCoordinate AStarVertex::parent() const {
  return parent_;
}
void AStarVertex::set_parent(const IdTimeCoordinate &parent) {
  parent_ = parent;
}

AStarVertex::IdTimeCoordinate AStarVertex::id_time_coordinate() const {
  return AStarVertex::IdTimeCoordinate(hex_vertex_id_, time_);
}




