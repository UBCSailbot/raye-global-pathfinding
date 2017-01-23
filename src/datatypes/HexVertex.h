// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXTILE_H
#define DATATYPES_HEXTILE_H

#include <Eigen/Dense>

/**
 * A HexVertex is a single vertex in a HexPlanet. It is a single vertex of the dual mesh.
 */
struct HexVertex {
  HexVertex(Eigen::Vector3f p);

  Eigen::Vector3f normal() const;

  /// The position of the vertex.
  Eigen::Vector3f vertex_position;
};

#endif  // DATATYPES_HEXTILE_H
