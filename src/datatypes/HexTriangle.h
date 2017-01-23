// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXTRI_H
#define DATATYPES_HEXTRI_H

#include <Eigen/Dense>
#include "HexVertex.h"

#include <cstdint>
#include <vector>

/**
 * A HexTriangle is an (triangle) element of the dual of the HexPlanet.
 */
struct HexTriangle {
  HexTriangle(uint32_t a, uint32_t b, uint32_t c);

  /// Indices into the vertex list
  uint32_t vertex_a, vertex_b, vertex_c;

  /**
   * Returns the center of the triangle.
   * @param vertices Set of all vertices in the mesh
   * @return The center of the triangle
   */
  Eigen::Vector3f GetCenter(const std::vector<HexVertex> &vertices);

  // Temporaries that don't need to stick around
  union {
    // Just used in the construction process
    uint32_t new_vertex_;

    // Angle around tile center
    float angle_;
  } tmp_;
};

#endif  // DATATYPES_HEXTRI_H
