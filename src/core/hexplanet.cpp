// Copyright 2017 UBC Sailbot

#include "hexplanet.h"

#include <map>
#include <set>
#include <cmath>
#include <iostream>
#include <sstream>

HexPlanet::HexPlanet() : m_subdLevel(0) {
}

HexPlanet::HexPlanet(int subd_level) : m_subdLevel(0) {
  // Build initial (level 0) mesh
  buildLevel0();

  // Subdivide until desired level
  while (m_subdLevel < subd_level) {
    subdivide();
  }

  // Planetize if we're at level 0
  if (subd_level == 0) {
    projectToSphere();
  }
}

float round_epsilon(float a) {
  return (fabs(a) < 1e-7) ? 0 : a;
}

void HexPlanet::write(std::ostream &o) {
  o << "# " << m_hexes.size() << " Vertices" << std::endl;
  for (std::vector<HexTile>::const_iterator i = m_hexes.begin(); i != m_hexes.end(); ++i) {
    const Eigen::Vector3f nrm = i->normal();
    o << 'v'
      << ' ' << round_epsilon(nrm[0])
      << ' ' << round_epsilon(nrm[1])
      << ' ' << round_epsilon(nrm[2])
      << std::endl;
  }

  o << "# " << m_triangles.size() << " Faces" << std::endl;
  for (std::vector<HexTri>::const_iterator i = m_triangles.begin(); i != m_triangles.end(); ++i) {
    o << 'f'
      << ' ' << i->m_hexA + 1
      << ' ' << i->m_hexB + 1
      << ' ' << i->m_hexC + 1
      << std::endl;
  }
}

void HexPlanet::read(std::istream &is) {
  std::string line;
  for (std::getline(is, line); !is.eof(); std::getline(is, line)) {
    std::istringstream iss(line);
    char firstChar;
    iss >> firstChar;
    if (firstChar == '#') {
      // comment - do nothing
    } else if (firstChar == 'v') {
      // vertex - 3 coordinates
      // make into a hex
      float x, y, z;
      iss >> x >> y >> z;
      m_hexes.push_back(HexTile(Eigen::Vector3f(x, y, z)));
    } else if (firstChar == 'f') {
      // face - 3 vert indices
      uint32_t x, y, z;
      iss >> x >> y >> z;
      m_triangles.push_back(HexTri(x - 1, y - 1, z - 1));
    }
  }
}

void HexPlanet::repairNormals() {
  for (std::vector<HexTri>::iterator i = m_triangles.begin(); i != m_triangles.end(); ++i) {
    // pull three verts of triangle
    const Eigen::Vector3f p0 = m_hexes[i->m_hexA].normal();
    const Eigen::Vector3f p1 = m_hexes[i->m_hexB].normal();
    const Eigen::Vector3f p2 = m_hexes[i->m_hexC].normal();
    Eigen::Vector3f n = (p2 - p0).cross(p1 - p0);
    n.normalize();
    const float d = n.dot(p0);
    if ((d >= 0 && d < 0.001) || (d < 0 && d > -0.001))
      std::cerr << "Triangle intersects origin" << std::endl;
    else if (d > 0) {
      // reverse points
      const uint32_t oldC = i->m_hexC;
      i->m_hexC = i->m_hexA;
      i->m_hexA = oldC;
    } else {
      // good
    }
  }
}

//=============================
// buildLevel0 -- builds the initial icosahedron
// for the planet mesh
//=============================
void HexPlanet::buildLevel0() {
  // hard code an icosahedron (20 sided die)
  m_hexes.erase(m_hexes.begin(), m_hexes.end());
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.723606f, 0.0f, 1.17082f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.0f, 1.17082f, 0.723606f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(-0.723606f, 0.0f, 1.17082f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.0f, -1.17082f, 0.723606f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.723606f, 0.0f, -1.17082f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.0f, -1.17082f, -0.723606f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(-0.723606f, 0.0f, -1.17082f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(0.0f, 1.17082f, -0.723606f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(1.17082f, -0.723606f, 0.0f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(1.17082f, 0.723606f, 0.0f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(-1.17082f, 0.723606f, 0.0f)));
  m_hexes.push_back(HexTile(Eigen::Vector3f(-1.17082f, -0.723606f, 0.0f)));

  m_triangles.push_back(HexTri(5, 11, 6));
  m_triangles.push_back(HexTri(1, 2, 0));
  m_triangles.push_back(HexTri(0, 2, 3));
  m_triangles.push_back(HexTri(5, 6, 4));
  m_triangles.push_back(HexTri(4, 6, 7));
  m_triangles.push_back(HexTri(9, 1, 0));
  m_triangles.push_back(HexTri(10, 2, 1));
  m_triangles.push_back(HexTri(2, 10, 11));
  m_triangles.push_back(HexTri(11, 3, 2));
  m_triangles.push_back(HexTri(8, 9, 0));
  m_triangles.push_back(HexTri(0, 3, 8));
  m_triangles.push_back(HexTri(11, 10, 6));
  m_triangles.push_back(HexTri(4, 7, 9));
  m_triangles.push_back(HexTri(9, 8, 4));
  m_triangles.push_back(HexTri(7, 6, 10));
  m_triangles.push_back(HexTri(1, 9, 7));
  m_triangles.push_back(HexTri(10, 1, 7));
  m_triangles.push_back(HexTri(5, 4, 8));
  m_triangles.push_back(HexTri(3, 11, 5));
  m_triangles.push_back(HexTri(8, 3, 5));

  // make planet sized
  // projectToSphere();
}

typedef std::map<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t> > AdjacencyMap;
void updateAdjacencyInfo(const std::pair<uint32_t, uint32_t> &edge, uint32_t triangleIndex, AdjacencyMap &am) {
  AdjacencyMap::iterator i = am.find(edge);
  if (i == am.end()) {
    am[edge] = std::make_pair(triangleIndex, -1);
  } else {
    // i->second is the value in the map (a pair of triangle indexes)
    // the first index was set in create (above)
    if (i->second.second != uint32_t(-1)) {
      std::cerr << "Error in updateAdjacencyInfo!" << std::endl;
    }
    i->second.second = triangleIndex;
  }
}

//=============================
// Perform sqrt(3) subdivision on the mesh
//=============================
void HexPlanet::subdivide() {
  // create two triangles in the new mesh for every edge in the src mesh

  // generate adjacency info
  AdjacencyMap adjacencyInfo;
  for (size_t ti = 0; ti != m_triangles.size(); ++ti) {
    const HexTri &t = m_triangles[ti];
    std::pair<uint32_t, uint32_t> eidAB(std::min(t.m_hexA, t.m_hexB), std::max(t.m_hexA, t.m_hexB));
    updateAdjacencyInfo(eidAB, ti, adjacencyInfo);

    std::pair<uint32_t, uint32_t> eidBC(std::min(t.m_hexB, t.m_hexC), std::max(t.m_hexB, t.m_hexC));
    updateAdjacencyInfo(eidBC, ti, adjacencyInfo);

    std::pair<uint32_t, uint32_t> eidCA(std::min(t.m_hexC, t.m_hexA), std::max(t.m_hexC, t.m_hexA));
    updateAdjacencyInfo(eidCA, ti, adjacencyInfo);
  }

  // foreach triangle in the old mesh, create a new vert at the center
  for (std::vector<HexTri>::iterator ti = m_triangles.begin();
       ti != m_triangles.end(); ti++) {
    // Create a new vert at the center of the triangle
    Eigen::Vector3f pNewVert;
    (*ti).m_tmp.m_newvert = static_cast<uint32_t> (m_hexes.size());
    pNewVert = (*ti).getCenter(m_hexes);

    // add it to the list of hexes
    m_hexes.push_back(HexTile(pNewVert));
  }

  // The new mesh that will be created
  std::vector<HexTri> newHexdual;

  // foreach edge, create two triangles
  for (AdjacencyMap::const_iterator ei = adjacencyInfo.begin(); ei != adjacencyInfo.end(); ++ei) {
    // given edge A,B - with neighbor across edge
    // first triangle is: A, center, neighbor's center
    // second triangle is: center, neighbor's center, B
    if (ei->second.first == uint32_t(-1) ||
        ei->second.second == uint32_t(-1)) {
      std::cerr << "Error in adjacency info" << std::endl;
      continue;
    }

    const uint32_t a = ei->first.first;
    const uint32_t b = ei->first.second;

    HexTri *t = &m_triangles[ei->second.first];
    HexTri *ot = &m_triangles[ei->second.second];

    newHexdual.push_back(HexTri(a, t->m_tmp.m_newvert, ot->m_tmp.m_newvert));
    newHexdual.push_back(HexTri(t->m_tmp.m_newvert, ot->m_tmp.m_newvert, b));
  }

  // replace the current set of hexes with the dual
  m_triangles = newHexdual;

  // reproject back to sphere
  projectToSphere();

  // note the subdivision
  m_subdLevel++;
}

//=============================
// projectToSphere()
//=============================
void HexPlanet::projectToSphere() {
  for (std::vector<HexTile>::iterator ti = m_hexes.begin();
       ti != m_hexes.end(); ti++) {
    Eigen::Vector3f p = (*ti).m_vertPos;
    p.normalize();
    (*ti).m_vertPos = p;
  }
}

size_t HexPlanet::getNumHexes() const {
  return m_hexes.size();
}

size_t HexPlanet::getHexIndexFromPoint(Eigen::Vector3f surfPos) {
  size_t best_hex = 0;
  float best_dot;

  // normalize
  Eigen::Vector3f p = surfPos;
  p.normalize();
  best_dot = acosf(m_hexes[0].m_vertPos.dot(p));


  // clever cheat -- just use the dot product to find the
  // smallest angle -- and thus the containing hex
  for (size_t ndx = 1; ndx < m_hexes.size(); ndx++) {
    float d = acosf(m_hexes[ndx].m_vertPos.dot(p));
    if (d < best_dot) {
      best_hex = ndx;
      best_dot = d;
    }
  }

  return best_hex;
}

//=============================
bool _cmpAngle(HexTri *a, HexTri *b) {
  return a->m_tmp.m_angle < b->m_tmp.m_angle;
}

// returns the polygon representation of this
// hex. Usually 6-sided but could be a pentagon	
void HexPlanet::getPolygon(uint32_t tileIndex, std::vector<Eigen::Vector3f> &poly) {
  // clear list
  poly.erase(poly.begin(), poly.end());

  // get neighboring hexes
  std::vector<uint32_t> neighbors;
  getNeighbors(tileIndex, neighbors);

  // sort edges to make a good polygon
  // ---first assign angles around center
  std::vector<HexTri *> triangles;
  const Eigen::Vector3f firstPos = m_triangles[neighbors[0]].getCenter(m_hexes);

  for (std::vector<uint32_t>::const_iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
    triangles.push_back(&m_triangles[*i]);
    Eigen::Vector3f v1 = firstPos - m_hexes[tileIndex].m_vertPos;
    Eigen::Vector3f nrm = m_triangles[*i].getCenter(m_hexes);
    Eigen::Vector3f v2 = nrm - m_hexes[tileIndex].m_vertPos;
    nrm.normalize();
    v1.normalize();
    v2.normalize();

    float ang = acosf(v1.dot(v2));
    Eigen::Vector3f t1 = v1.cross(v2);
    const float dir = nrm.dot(t1);
    if (dir < 0.0f) ang = static_cast<float> (M_PI + (M_PI - ang));

    m_triangles[*i].m_tmp.m_angle = ang;
  }
  std::sort(triangles.begin(), triangles.end(), _cmpAngle);

  // construct polygon
  for (std::vector<HexTri *>::iterator ti = triangles.begin(); ti != triangles.end(); ++ti) {
    Eigen::Vector3f p = (**ti).getCenter(m_hexes);
    p.normalize();
    poly.push_back(p);
  }
}

// returns the indices of the neighbors of this tile
// Usually 6, could be 5
void HexPlanet::getNeighbors(uint32_t tileNdx, std::vector<uint32_t> &nbrs) const {
  // clear list
  nbrs.erase(nbrs.begin(), nbrs.end());

  std::set<uint32_t> candidates;

  // find neighbors
  for (size_t ti = 0; ti != m_triangles.size(); ++ti) {
    if (m_triangles[ti].m_hexA == tileNdx ||
        m_triangles[ti].m_hexB == tileNdx ||
        m_triangles[ti].m_hexC == tileNdx) {
      candidates.insert(ti);
    }
  }

  nbrs.insert(nbrs.end(), candidates.begin(), candidates.end());
}

// Returns a point on the planet's surface given a ray
bool HexPlanet::rayHitPlanet(Eigen::Vector3f p, Eigen::Vector3f dir, Eigen::Vector3f &result) {
  float a, b, c, d;
  a = dir.dot(dir);
  b = (2.0f * dir).dot(p);
  c = p.dot(p) - 1;
  d = b * b - 4.0f * a * c;
  if (d <= 0) return false;
  result = p + ((-b - sqrtf(d)) / 2.0f * a) * dir;
  return true;
}
