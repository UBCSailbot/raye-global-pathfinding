// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarVertex.h"

AStarVertex::AStarVertex(HexVertexId hex_vertex_id,
                         uint32_t time,
                         double cost,
                         double heuristic_cost,
                         IdTimeIndex parent)
    : id_time_index_({hex_vertex_id, time}), cost_(cost), heuristic_cost_(heuristic_cost), parent_(parent) {}

bool AStarVertex::operator<(const AStarVertex &rhs) const {
  return cost_ + heuristic_cost_ > rhs.cost() + rhs.heuristic_cost();
}
HexVertexId AStarVertex::hex_vertex_id() const {
  return id_time_index_.first;
}
uint32_t AStarVertex::time() const {
  return id_time_index_.second;
}
double AStarVertex::cost() const {
  return cost_;
}
AStarVertex::IdTimeIndex AStarVertex::parent() const {
  return parent_;
}
void AStarVertex::set_parent(const IdTimeIndex &parent) {
  parent_ = parent;
}

AStarVertex::IdTimeIndex AStarVertex::id_time_index() const {
  return id_time_index_;
}

double AStarVertex::heuristic_cost() const {
  return heuristic_cost_;
}




