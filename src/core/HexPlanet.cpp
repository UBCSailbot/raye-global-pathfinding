// Copyright 2017 UBC Sailbot

#include "HexPlanet.h"

#include <map>
#include <set>
#include <cmath>
#include <iostream>
#include <sstream>

HexPlanet::HexPlanet(int subdivision_level) {
  // Build initial (level 0) mesh
  build_level_0();

  // Subdivide until desired level
  while (subdivision_level_ < subdivision_level) {
    Subdivide();
  }

  // Planetize if we're at level 0
  if (subdivision_level == 0) {
    ProjectToSphere();
  }

  RepairNormals();
}

float round_epsilon(float a) {
  return (fabs(a) < 1e-7) ? 0 : a;
}

void HexPlanet::Write(std::ostream &o) {
  o << "# " << vertices_.size() << " Vertices" << std::endl;
  for (std::vector<HexVertex>::const_iterator i = vertices_.begin(); i != vertices_.end(); ++i) {
    const Eigen::Vector3f normal = i->normal();
    o << 'v'
      << ' ' << round_epsilon(normal[0])
      << ' ' << round_epsilon(normal[1])
      << ' ' << round_epsilon(normal[2])
      << std::endl;
  }

  o << "# " << triangles_.size() << " Faces" << std::endl;
  for (std::vector<HexTriangle>::const_iterator i = triangles_.begin(); i != triangles_.end(); ++i) {
    o << 'f'
      << ' ' << i->vertex_a + 1
      << ' ' << i->vertex_b + 1
      << ' ' << i->vertex_c + 1
      << std::endl;
  }
}

void HexPlanet::Read(std::istream &is) {
  std::string line;
  for (std::getline(is, line); !is.eof(); std::getline(is, line)) {

    std::istringstream iss(line);
    char firstChar;
    iss >> firstChar;

    if (firstChar == '#') {
      // Comment - do nothing
    } else if (firstChar == 'v') {
      // Vertex - 3 coordinates (make a hex)
      float x, y, z;
      iss >> x >> y >> z;
      vertices_.push_back(HexVertex(Eigen::Vector3f(x, y, z)));
    } else if (firstChar == 'f') {
      // Face - 3 vert indices
      uint32_t x, y, z;
      iss >> x >> y >> z;
      triangles_.push_back(HexTriangle(x - 1, y - 1, z - 1));
    }
  }
}

void HexPlanet::RepairNormals() {
  for (HexTriangle &triangle : triangles_) {
    // Pull three vertices of the triangle
    const Eigen::Vector3f p0 = vertices_[triangle.vertex_a].normal();
    const Eigen::Vector3f p1 = vertices_[triangle.vertex_b].normal();
    const Eigen::Vector3f p2 = vertices_[triangle.vertex_c].normal();

    Eigen::Vector3f n = (p2 - p0).cross(p1 - p0);
    n.normalize();
    const float d = n.dot(p0);

    if ((d >= 0 && d < 0.001) || (d < 0 && d > -0.001)) {
      std::cerr << "Triangle intersects origin" << std::endl;
    } else if (d > 0) {
      // Reverse points
      const uint32_t oldC = triangle.vertex_c;
      triangle.vertex_c = triangle.vertex_a;
      triangle.vertex_a = oldC;
    } else {
      // No correction necessary
    }
  }
}

void HexPlanet::build_level_0() {
  // Hard code an icosahedron (20 sided die)
  vertices_.reserve(12);
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.723606f, 0.0f, 1.17082f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.0f, 1.17082f, 0.723606f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(-0.723606f, 0.0f, 1.17082f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.0f, -1.17082f, 0.723606f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.723606f, 0.0f, -1.17082f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.0f, -1.17082f, -0.723606f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(-0.723606f, 0.0f, -1.17082f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(0.0f, 1.17082f, -0.723606f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(1.17082f, -0.723606f, 0.0f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(1.17082f, 0.723606f, 0.0f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(-1.17082f, 0.723606f, 0.0f)));
  vertices_.push_back(HexVertex(Eigen::Vector3f(-1.17082f, -0.723606f, 0.0f)));

  triangles_.reserve(20);
  triangles_.push_back(HexTriangle(5, 11, 6));
  triangles_.push_back(HexTriangle(1, 2, 0));
  triangles_.push_back(HexTriangle(0, 2, 3));
  triangles_.push_back(HexTriangle(5, 6, 4));
  triangles_.push_back(HexTriangle(4, 6, 7));
  triangles_.push_back(HexTriangle(9, 1, 0));
  triangles_.push_back(HexTriangle(10, 2, 1));
  triangles_.push_back(HexTriangle(2, 10, 11));
  triangles_.push_back(HexTriangle(11, 3, 2));
  triangles_.push_back(HexTriangle(8, 9, 0));
  triangles_.push_back(HexTriangle(0, 3, 8));
  triangles_.push_back(HexTriangle(11, 10, 6));
  triangles_.push_back(HexTriangle(4, 7, 9));
  triangles_.push_back(HexTriangle(9, 8, 4));
  triangles_.push_back(HexTriangle(7, 6, 10));
  triangles_.push_back(HexTriangle(1, 9, 7));
  triangles_.push_back(HexTriangle(10, 1, 7));
  triangles_.push_back(HexTriangle(5, 4, 8));
  triangles_.push_back(HexTriangle(3, 11, 5));
  triangles_.push_back(HexTriangle(8, 3, 5));
}

typedef std::map<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t> > AdjacencyMap;
void UpdateAdjacencyInfo(const std::pair<uint32_t, uint32_t> &edge, uint32_t triangle_index, AdjacencyMap &am) {
  AdjacencyMap::iterator i = am.find(edge);
  if (i == am.end()) {
    am[edge] = std::make_pair(triangle_index, -1);
  } else {
    // i->second is the value in the map (a pair of triangle indexes)
    // the first index was set in create (above)
    if (i->second.second != uint32_t(-1)) {
      std::cerr << "Error in UpdateAdjacencyInfo!" << std::endl;
    }
    i->second.second = triangle_index;
  }
}

void HexPlanet::Subdivide() {
  // Generate adjacency info
  AdjacencyMap adjacency_info;
  for (uint32_t ti = 0; ti != triangles_.size(); ++ti) {
    const HexTriangle &t = triangles_[ti];
    std::pair<uint32_t, uint32_t> edge_a_b(std::min(t.vertex_a, t.vertex_b), std::max(t.vertex_a, t.vertex_b));
    UpdateAdjacencyInfo(edge_a_b, ti, adjacency_info);

    std::pair<uint32_t, uint32_t> edge_b_c(std::min(t.vertex_b, t.vertex_c), std::max(t.vertex_b, t.vertex_c));
    UpdateAdjacencyInfo(edge_b_c, ti, adjacency_info);

    std::pair<uint32_t, uint32_t> edge_c_a(std::min(t.vertex_c, t.vertex_a), std::max(t.vertex_c, t.vertex_a));
    UpdateAdjacencyInfo(edge_c_a, ti, adjacency_info);
  }

  // For each triangle in the old mesh, create a new vertex at the center
  // Since we know how many elements we will have, resize the vector ahead of time to avoid multiple allocations
  triangles_.reserve(2 * triangles_.size());
  for (HexTriangle &ti : triangles_) {
    // Create a new vert at the center of the triangle
    ti.tmp_.new_vertex_ = static_cast<uint32_t> (vertices_.size());
    Eigen::Vector3f new_vertex = ti.GetCenter(vertices_);

    // Add it to the list of hexes
    vertices_.push_back(HexVertex(new_vertex));
  }

  // The new mesh that will be created
  std::vector<HexTriangle> new_hex_dual;
  // As with the vertices, allocate the memory once
  new_hex_dual.reserve(2 * adjacency_info.size());

  // For each edge, create two triangles
  for (auto ei : adjacency_info) {
    // Given edge A,B - with neighbor across edge
    // First triangle is: A, center, neighbor's center
    // Second triangle is: center, neighbor's center, B
    if (ei.second.first == uint32_t(-1) ||
        ei.second.second == uint32_t(-1)) {
      std::cerr << "Error in adjacency info" << std::endl;
      continue;
    }

    const uint32_t a = ei.first.first;
    const uint32_t b = ei.first.second;

    HexTriangle *t = &triangles_[ei.second.first];
    HexTriangle *ot = &triangles_[ei.second.second];

    new_hex_dual.push_back(HexTriangle(a, t->tmp_.new_vertex_, ot->tmp_.new_vertex_));
    new_hex_dual.push_back(HexTriangle(t->tmp_.new_vertex_, ot->tmp_.new_vertex_, b));
  }

  // Replace the current set of hexes with the dual
  triangles_ = new_hex_dual;

  // Project back to sphere (push out new vertices)
  ProjectToSphere();

  // Note the subdivision
  subdivision_level_++;
}

void HexPlanet::ProjectToSphere() {
  for (std::vector<HexVertex>::iterator ti = vertices_.begin(); ti != vertices_.end(); ti++) {
    Eigen::Vector3f p = (*ti).vertex_position;
    p.normalize();
    (*ti).vertex_position = p;
  }
}

size_t HexPlanet::HexIndexFromPoint(Eigen::Vector3f surface_position) {
  size_t best_hex = 0;
  float best_dot;

  // Normalize
  surface_position.normalize();
  best_dot = acosf(vertices_[0].vertex_position.dot(surface_position));

  // Clever cheat -- just use the dot product to find the smallest angle -- and thus the containing hex
  for (size_t i = 1; i < vertices_.size(); i++) {
    float d = acosf(vertices_[i].vertex_position.dot(surface_position));
    if (d < best_dot) {
      best_hex = i;
      best_dot = d;
    }
  }

  return best_hex;
}

void HexPlanet::GetPolygon(uint32_t tile_index, std::vector<Eigen::Vector3f> *polygon) {
  // Clear list
  polygon->erase(polygon->begin(), polygon->end());

  // Get neighboring hexes
  std::vector<uint32_t> neighbors;
  GetNeighbors(tile_index, &neighbors);

  // Sort edges to make a good polygon (first assign angles around center)
  std::vector<HexTriangle *> triangles;
  const Eigen::Vector3f firstPos = triangles_[neighbors[0]].GetCenter(vertices_);

  for (std::vector<uint32_t>::const_iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
    triangles.push_back(&triangles_[*i]);
    Eigen::Vector3f v1 = firstPos - vertices_[tile_index].vertex_position;
    Eigen::Vector3f normal = triangles_[*i].GetCenter(vertices_);
    Eigen::Vector3f v2 = normal - vertices_[tile_index].vertex_position;
    normal.normalize();
    v1.normalize();
    v2.normalize();

    float ang = acosf(v1.dot(v2));
    Eigen::Vector3f t1 = v1.cross(v2);
    const float dir = normal.dot(t1);
    if (dir < 0.0f) ang = static_cast<float> (M_PI + (M_PI - ang));

    triangles_[*i].tmp_.angle_ = ang;
  }

  std::sort(triangles.begin(),
            triangles.end(),
            [](HexTriangle *a, HexTriangle *b) { return a->tmp_.angle_ < b->tmp_.angle_; });

  // Construct polygons
  for (std::vector<HexTriangle *>::iterator ti = triangles.begin(); ti != triangles.end(); ++ti) {
    Eigen::Vector3f p = (**ti).GetCenter(vertices_);
    p.normalize();
    polygon->push_back(p);
  }
}

void HexPlanet::GetNeighbors(uint32_t tile_index, std::vector<uint32_t> *neighbors) const {
  // Clear list
  neighbors->erase(neighbors->begin(), neighbors->end());

  std::set<uint32_t> candidates;

  // Find neighbors
  for (size_t ti = 0; ti < triangles_.size(); ti++) {
    if (triangles_[ti].vertex_a == tile_index ||
        triangles_[ti].vertex_b == tile_index ||
        triangles_[ti].vertex_c == tile_index) {
      candidates.insert(ti);
    }
  }

  neighbors->insert(neighbors->end(), candidates.begin(), candidates.end());
}

bool HexPlanet::RayHitPlanet(const Eigen::Vector3f &p, const Eigen::Vector3f &dir, Eigen::Vector3f *result) {
  float a, b, c, d;
  a = dir.dot(dir);
  b = (2.0f * dir).dot(p);
  c = p.dot(p) - 1;
  d = b * b - 4.0f * a * c;

  if (d <= 0) {
    return false;
  } else {
    *result = p + ((-b - sqrtf(d)) / 2.0f * a) * dir;
    return true;
  }
}
