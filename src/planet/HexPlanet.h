// Copyright 2017 UBC Sailbot

#ifndef PLANET_HEXPLANET_H_
#define PLANET_HEXPLANET_H_

#include <cstdint>

#include <vector>
#include <array>

#include <Eigen/Dense>

#include "datatypes/GPSCoordinate.h"
#include "datatypes/GPSCoordinateFast.h"
#include "datatypes/HexTriangle.h"
#include "datatypes/HexVertex.h"

/**
 * A planet mesh in which verticies have 6 neighbours (sometimes 5), making it look like it's composed of hex tiles.
 */
class HexPlanet {
 public:
  explicit HexPlanet(int subdivision_level);

  /**
   * @return The number of vertices in the planet mesh.
   */
  size_t vertex_count() const { return vertices_.size(); }

  /**
   * @return The number of triangles that compose the planet mesh.
   */
  size_t triangle_count() const { return triangles_.size(); }

  /**
   * @param vertex_index Desired vertex index
   * @return Reference to vertex
   */
  const HexVertex &vertex(HexVertexId vertex_index) const { return vertices_[vertex_index]; }

  /**
   * @param triangle_index Desired triangle index
   * @return Reference to triangle
   */
  const HexTriangle &triangle(size_t triangle_index) const { return triangles_[triangle_index]; }

  /**
   * Efficiently update all vertex neighbours.
   */
  void UpdateVertexNeighbours();

  /**
   * Returns the indices of the neighbours of this vertex.
   * Note: Usually 6 but can be 5. No particular order.
   * @param vertex_index Target vertex index
   * @param neighbours Destination array
   */
  void GetNeighbours(HexVertexId vertex_index, std::array<HexVertexId, 6> *neighbours) const;

  /**
   * Returns a point on the planet's surface given a ray
   * @param p Ray origin
   * @param dir Ray direction
   * @param result Destination point
   * @return Whether the operation was successful
   */
  bool RayHitPlanet(const Eigen::Vector3f &p, const Eigen::Vector3f &dir, Eigen::Vector3f *result);

  /**
   * Get a hex index from a 3D point.
   * Projects the point down to the planet surface and returns the index of the hex containing it.
   * @param surface_position Position on the surface of the planet.
   * @return The index of the nearest hex.
   */
  size_t HexIndexFromPoint(Eigen::Vector3f surface_position);

  /**
   * Get a get a GPS Coordinate from a vertex id
   * @param HexVertexId Hex Vertex Id on planet
   * @return GPSCoordinate of that index
   */
  GPSCoordinateFast GPSCoordinateFromHexIndex(HexVertexId id) const;

  /**
   * Write the planet mesh to an output stream.
   * @param o Target output stream
   */
  void Write(std::ostream &o);
  /**
   * Read the planet mesh from an input stream.
   * @param i Target input stream
   */
  void Read(std::istream &i);

 protected:
  /// Current subdivision level (0 is an icosahedron).
  int subdivision_level_ = 0;

  /// Vertices
  std::vector<HexVertex> vertices_;
  /// Triangles (and thus the edges)
  std::vector<HexTriangle> triangles_;

  /**
   * Builds the initial icosahedron (20 sided die).
   * This shape is later subdivided if necessary.
   */
  void build_level_0();

  /**
   * Repair edge normals.
   * Note: This should be performed after subdivision.
   */
  void RepairNormals();

  /**
   * Perform sqrt(3) subdivision on the planet mesh.
   * Creates two triangles in the new mesh for every edge in the original mesh.
   * Note: Normals should be repaired after subdivision are complete
   */
  void Subdivide();

  /**
   * Project all vertices back onto the sphere.
   * This pushes out new vertices.
   */
  void ProjectToSphere();
};

#endif  // PLANET_HEXPLANET_H_
