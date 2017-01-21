// Copyright 2017 UBC Sailbot

#ifndef HEXPLANET_H
#define HEXPLANET_H

#include <datatypes/HexTri.h>
#include <datatypes/HexTile.h>

#include <stdint.h>
#include <vector>

#include <Eigen/Dense>

class HexPlanet {
 public:
  HexPlanet();
  HexPlanet(int subd_level);

  void write(std::ostream &o);
  void read(std::istream &i);

  void repairNormals();

  size_t getNumHexes() const;
  size_t numTriangles() const { return m_triangles.size(); }

  void subdivide();

  // returns the polygon representation of this
  // hex. Usually 6-sided but could be a pentagon
  void getPolygon(uint32_t tileIndex, std::vector <Eigen::Vector3f> &poly);

  // returns the indices of the neighbors of this tile
  // Usually 6, could be 5. These aren't in any order
  void getNeighbors(uint32_t tileNdx, std::vector <uint32_t> &nbrs) const;

  // Returns a point on the planet's surface given a ray
  bool rayHitPlanet(Eigen::Vector3f p, Eigen::Vector3f dir, Eigen::Vector3f &result);

  // Get a hex index from a 3d point .. projects the point
  // down to the planet surface and returns the index of the
  // hex containing it
  size_t getHexIndexFromPoint(Eigen::Vector3f surfPos);

  const HexTri &triangle(size_t triIndex) const { return m_triangles[triIndex]; }
  const HexTile &hex(size_t hexIndex) const { return m_hexes[hexIndex]; }

  void projectToSphere();

 protected:
  int m_subdLevel;

  // construction operations
  void buildLevel0();

  // data
  std::vector <HexTile> m_hexes;
  std::vector <HexTri> m_triangles;
};

#endif
