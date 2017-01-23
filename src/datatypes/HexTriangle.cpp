// Copyright 2017 UBC Sailbot

#include "HexTriangle.h"

HexTriangle::HexTriangle(uint32_t a, uint32_t b, uint32_t c) :
    vertex_a(a), vertex_b(b), vertex_c(c) {
  // Mark newvert as uninitialized
  tmp_.new_vertex_ = -1;
}

Eigen::Vector3f HexTriangle::GetCenter(const std::vector<HexVertex> &vertices) {
  Eigen::Vector3f center_point = vertices[vertex_a].vertex_position;
  center_point += vertices[vertex_b].vertex_position;
  center_point += vertices[vertex_c].vertex_position;
  center_point /= 3.0f;

  return center_point;
}