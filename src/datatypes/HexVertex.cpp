// Copyright 2017 UBC Sailbot

#include "datatypes/HexVertex.h"

HexVertex::HexVertex(Eigen::Vector3f p) : vertex_position(p) {}

Eigen::Vector3f HexVertex::normal() const {
  return vertex_position.normalized();
}
