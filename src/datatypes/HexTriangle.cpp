// Copyright 2017 UBC Sailbot

#include "datatypes/HexTriangle.h"

HexTriangle::HexTriangle(HexVertexId a, HexVertexId b, HexVertexId c) : vertex_a(a), vertex_b(b), vertex_c(c) {
  // Mark new_vertex_ as uninitialized
  tmp_.new_vertex_ = kInvalidHexVertexId;
}

Eigen::Vector3f HexTriangle::GetCenter(const std::vector<HexVertex> &vertices) {
  Eigen::Vector3f center_point = vertices[vertex_a].vertex_position;
  center_point += vertices[vertex_b].vertex_position;
  center_point += vertices[vertex_c].vertex_position;
  center_point /= 3.0f;

  return center_point;
}
