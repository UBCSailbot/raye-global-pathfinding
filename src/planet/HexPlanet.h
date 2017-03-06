// Copyright 2017 UBC Sailbot

#ifndef PLANET_HEXPLANET_H_
#define PLANET_HEXPLANET_H_

#include <cstdint>

#include <vector>
#include <array>

#include <Eigen/Dense>
#include <unordered_map>
#include <boost/unordered_map.hpp>

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
   * Get a hex vertex index from a 3D point.
   * Projects the point down to the planet surface and returns the index of the hex containing it.
   * @param surface_position Position on the surface of the planet.
   * @return The index of the nearest hex.
   */
  HexVertexId HexVertexFromPoint(Eigen::Vector3f surface_position);

  /**
   * Get a get a GPS Coordinate from a vertex ID.
   * Note: The results are cached for improved performance. The returned reference is safe to hold for the lifetime of
   * the planet.
   * @param HexVertexId Hex Vertex ID on planet.
   * @return GPSCoordinate of that index.
   */
  const GPSCoordinateFast &GPSCoordinateFromHexVertex(HexVertexId id);

  /**
   * Get the distance (in meters) between two vertices as computed by the Haversine formula.
   * Note: The results are cached for improved performance.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @return Distance in meters between soure and target.
   */
  uint32_t DistanceBetweenVertices(HexVertexId source, HexVertexId target);

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
   * Vertex coordinate mapping.
   * This is stored separate from the HexVertexes since these are computed on the fly as needed.
   */
  std::unordered_map<HexVertexId, GPSCoordinateFast> vertex_coordinates_;

  /**
   * Distances cache.
   * Stores the distance between two vertices keyed a source/target pair as computed by the Haversine formula.
   */
  boost::unordered_map<std::pair<HexVertexId, HexVertexId>, uint32_t> vertex_distances_;

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
