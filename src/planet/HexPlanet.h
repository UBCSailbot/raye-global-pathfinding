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
 * @brief A planet mesh in which most vertices have 6 neighbours, making it look like it's composed of hex tiles.
 * Note: All vertices have either 6 or 5 neighbours.
 */
class HexPlanet {
 public:
  /// A pair of HexVertexId.
  typedef const std::pair<HexVertexId, HexVertexId> HexVertexPair;
  /// A mapping between edges (HexVertexPair) and a pair of triangles
  typedef std::map<HexVertexPair, std::pair<uint32_t, uint32_t> > AdjacencyMap;

  /**
   * Create a HexPlanet with the given subdivision level.
   * @param subdivision_level The subdivision count.
   */
  explicit HexPlanet(int subdivision_level);

  /**
   * @return The number of subdivisions used to generate the planet.
   */
  int subdivision_level() const { return subdivision_level_; }

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
   * Get the distance (in meters) between two vertices as computed by the Haversine formula.
   * @warning Use |vertex(source).neighbour_distances| when possible!
   * Note: The results are cached for improved performance.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @return Distance in meters between source and target.
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
   * Distance cache.
   * Stores the distance between two vertices keyed a source/target pair as computed by the Haversine formula.
   * @warning this is not to be used for storing distances between direct neighbours.
   */
  boost::unordered_map<HexVertexPair, uint32_t> vertex_distances_;

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
   * Note: Normals should be repaired after subdivision are complete.
   */
  void Subdivide();

  /**
   * Project all vertices back onto the sphere.
   * This pushes out new vertices.
   */
  void ProjectToSphere();

  /**
   * Compute (and cache) the GPS Coordinates of each vertex.
   */
  void ComputeVertexCoordinates();

  /**
   * Efficiently compute (and cache) all vertex neighbours.
   */
  void ComputeVertexNeighbours();

  /**
   * Compute (and cache) the distance to the neighbours of each vertex.
   */
  void ComputeVertexNeighbourDistances();

  /**
   * Add one of two triangles that are associated with each edge.
   * @param edge Edge to associate.
   * @param triangle_index Triangle index to register.
   * @param adjacency_map A map between edges and triangle pairs to update.
   */
  void UpdateAdjacencyInfo(const HexVertexPair &edge, uint32_t triangle_index, AdjacencyMap &adjacency_map);
};

#endif  // PLANET_HEXPLANET_H_
