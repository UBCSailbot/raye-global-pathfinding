// Copyright 2017 UBC Sailbot

#ifndef HEXPLANET_H
#define HEXPLANET_H

#include <datatypes/HexTriangle.h>
#include <datatypes/HexVertex.h>

#include <stdint.h>
#include <vector>

#include <Eigen/Dense>

/**
 * A planet mesh in which verticies have 6 neighbors (sometimes 5), making it look like it's composed of hex tiles.
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
  const HexVertex &vertex(size_t vertex_index) const { return vertices_[vertex_index]; }

  /**
   * @param triangle_index Desired triangle index
   * @return Reference to triangle
   */
  const HexTriangle &triangle(size_t triangle_index) const { return triangles_[triangle_index]; }

  /**
   * Returns the polygon representation of a given tile.
   * Note: Usually 6-sided but could be a pentagon. No particular order.
   * @param tile_index Target hex index
   * @param polygon Destination vector
   */
  void GetPolygon(uint32_t tile_index, std::vector<Eigen::Vector3f> *polygon);

  /**
   * Returns the indices of the neighbors of this tile.
   * Note: Usually 6-sided but could be a pentagon. No particular order.
   * @param tile_index Target hex index
   * @param neighbors Destination vector
   */
  void GetNeighbors(uint32_t tile_index, std::vector<uint32_t> *neighbors) const;

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

#endif
