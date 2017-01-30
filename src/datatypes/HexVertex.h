// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXTILE_H
#define DATATYPES_HEXTILE_H

#include "HexDefs.h"

#include <Eigen/Dense>
#include <array>

/**
 * A HexVertex is a single vertex in a HexPlanet. It is a single vertex of the dual mesh.
 */
struct HexVertex {
  static constexpr HexVertexId kMaxHexVertexNeighbourCount = 6;
  HexVertex(Eigen::Vector3f p);

  Eigen::Vector3f normal() const;

  /// The position of the vertex.
  Eigen::Vector3f vertex_position;

  /**
   * Array of neighbouring vertex IDs.
   * When initialized, this contains 5 or more commonly 6 valid IDs.
   * Invalid entries are set to kInvalidHexVertexId.
   */
  std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbours =
      {{kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId,
        kInvalidHexVertexId}};
};

#endif  // DATATYPES_HEXTILE_H