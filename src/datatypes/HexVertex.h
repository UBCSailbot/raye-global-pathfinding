// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXVERTEX_H_
#define DATATYPES_HEXVERTEX_H_

#include "HexDefs.h"

#include <Eigen/Dense>
#include <array>

/**
 * A HexVertex is a single vertex in a HexPlanet. It is a single vertex of the dual mesh.
 */
class HexVertex {
 public:
  /// The maximum number of neighbours that any vertex can have in a valid HexPlanet.
  static constexpr HexVertexId kMaxHexVertexNeighbourCount = 6;

  explicit HexVertex(Eigen::Vector3f p);

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

  HexVertexId neighbour_count = 0;
};

#endif  // DATATYPES_HEXVERTEX_H_
