// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXVERTEX_H_
#define DATATYPES_HEXVERTEX_H_

#include <array>

#include <Eigen/Dense>
#include <vector>

#include "datatypes/HexDefs.h"
#include "datatypes/GPSCoordinateFast.h"

/**
 * A HexVertex is a single vertex in a HexPlanet. It is a single vertex of the dual mesh.
 */
class HexVertex {
 public:
  /// The maximum number of neighbours that any vertex can have in a valid HexPlanet.
  static constexpr HexVertexId kMaxHexVertexNeighbourCount = 6;

  explicit HexVertex(Eigen::Vector3f p);
  explicit HexVertex(Eigen::Vector3f vertex_position__, GPSCoordinateFast gps__, std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbours__, std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbour_distances__, HexVertexId neighbour_count__, std::vector<HexVertexId> indirect_neighbours__);

  Eigen::Vector3f normal() const { return vertex_position.normalized(); }

  /// The position of the vertex.
  Eigen::Vector3f vertex_position;

  /// The GPS coordinate of the vertex.
  GPSCoordinateFast coordinate;

  /**
   * Array of neighbouring vertex IDs.
   * When initialized, this contains 5 or more commonly 6 valid IDs.
   * Invalid entries are set to kInvalidHexVertexId.
   */
  std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbours =
      {{kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId, kInvalidHexVertexId,
        kInvalidHexVertexId}};

  /**
   * Array of the distances to neighbouring vertices, whose IDs are stored in |neighbours|.
   * When initialized, this contains 5 or more commonly 6 valid distances.
   */
  std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbour_distances = {{0, 0, 0, 0, 0, 0}};

  /**
   * Storage of indirect neighbours.
   * These are used to increase the angular resolution of pathfinding that's possible by making distant neighbours
   * accessible. Because of the HexPlanet's geometry this greatly increases the quality of the path since it reduces
   * the need to zig-zag.
   */
  std::vector<HexVertexId> indirect_neighbours;

  HexVertexId neighbour_count = 0;
};

#endif  // DATATYPES_HEXVERTEX_H_
