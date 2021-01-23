// Copyright 2017 UBC Sailbot

#include "planet/HexPlanet.h"
#include "common/ProgressBar.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "logic/StandardCalc.h"

HexPlanet::HexPlanet(uint8_t subdivision_level, uint8_t indirect_neighbour_depth) {
  // Setup for progress bar
  ProgressBar progress_bar;
  int total_num_steps = 8;  // Number of major steps in this function
  int current_step = 0;
  progress_bar.update((double)(current_step) / total_num_steps);
  progress_bar.print(" | Building level 0...");

  // Build initial (level 0) mesh
  build_level_0();
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Subdividing...");

  // Subdivide until desired level
  while (subdivision_level_ < subdivision_level) {
    Subdivide();
  }
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Projecting to sphere...");

  // Planetize if we're at level 0
  if (subdivision_level == 0) {
    ProjectToSphere();
  }
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Repairing normals...");

  RepairNormals();
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Computing vertex coordinates...");

  // Initialize the coordinate for each vertex
  ComputeVertexCoordinates();
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Computing vertex neighbours...");

  // Initialize the neighbours for each vertex
  ComputeVertexNeighbours();
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Computing neighbour distances...");

  // Initialize the neighbour distances for each vertex
  ComputeVertexNeighbourDistances();
  progress_bar.update((double)(++current_step) / total_num_steps);
  progress_bar.print(" | Computing indirect vertex neighbours...");

  // Initialize the indirect (but close) neighbours for each vertex
  ComputeIndirectVertexNeighbours(indirect_neighbour_depth);
  progress_bar.flush();
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

void HexPlanet::UpdateAdjacencyInfo(const std::pair<uint32_t, uint32_t> &edge,
                                    uint32_t triangle_index,
                                    HexPlanet::AdjacencyMap &adjacency_map) {
  auto i = adjacency_map.find(edge);
  if (i == adjacency_map.end()) {
    // Add the edge to the adjacency map (record the first triangle).
    adjacency_map[edge] = std::make_pair(triangle_index, -1);
  } else {
    // Record the second triangle that's attached to the edge.
    // i->second is the value in the map (a pair of triangle indexes)
    // the first index was set in create (above)
    if (i->second.second != kInvalidHexVertexId) {
      throw std::runtime_error("Adjacency map contains kInvalidHexVertexIdError");
    }
    i->second.second = triangle_index;
  }
}

void HexPlanet::Subdivide() {
  // Generate adjacency info
  AdjacencyMap adjacency_info;
  for (uint32_t ti = 0; ti != triangles_.size(); ++ti) {
    const HexTriangle &t = triangles_[ti];
    HexVertexPair edge_a_b(std::min(t.vertex_a, t.vertex_b), std::max(t.vertex_a, t.vertex_b));
    UpdateAdjacencyInfo(edge_a_b, ti, adjacency_info);

    HexVertexPair edge_b_c(std::min(t.vertex_b, t.vertex_c), std::max(t.vertex_b, t.vertex_c));
    UpdateAdjacencyInfo(edge_b_c, ti, adjacency_info);

    HexVertexPair edge_c_a(std::min(t.vertex_c, t.vertex_a), std::max(t.vertex_c, t.vertex_a));
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
    // Given edge A, B - with neighbour across edge
    // First triangle is: A, center, neighbour's center
    // Second triangle is: center, neighbour's center, B
    if (ei.second.first == kInvalidHexVertexId || ei.second.second == kInvalidHexVertexId) {
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
  for (std::vector<HexVertex>::iterator ti = vertices_.begin(); ti != vertices_.end(); ++ti) {
    Eigen::Vector3f p = (*ti).vertex_position;
    p.normalize();
    (*ti).vertex_position = p;
  }
}

HexVertexId HexPlanet::HexVertexFromPoint(Eigen::Vector3f surface_position) {
  HexVertexId best_hex = 0;
  float best_dot;

  // Normalize
  surface_position.normalize();
  best_dot = acosf(vertices_[0].vertex_position.dot(surface_position));

  // Clever cheat -- just use the dot product to find the smallest angle -- and thus the containing hex
  for (HexVertexId i = 1; i < vertices_.size(); i++) {
    float d = acosf(vertices_[i].vertex_position.dot(surface_position));
    if (d < best_dot) {
      best_hex = i;
      best_dot = d;
    }
  }

  return best_hex;
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

uint32_t HexPlanet::DistanceBetweenVertices(HexVertexId source, HexVertexId target) {
  if (source == target) {
    return 0;
  }
  std::pair<HexVertexId, HexVertexId> key = {source, target};
  // Check if the distance has already been computed.
  auto search = vertex_distances_.find(key);
  if (search != vertex_distances_.end()) {
    // The distance already exists, return it.
    return search->second;
  } else {
    // Compute the distance, save it, return it.
    const GPSCoordinateFast &source_coord = vertices_[source].coordinate;
    const GPSCoordinateFast &target_coord = vertices_[target].coordinate;
    uint32_t distance = standard_calc::DistBetweenTwoCoords(source_coord, target_coord);
    vertex_distances_.insert({key, distance});
    return distance;
  }
}

void HexPlanet::ComputeVertexCoordinates() {
  for (HexVertex &vertex : vertices_) {
    Eigen::Vector3f cartesian = vertex.normal();
    vertex.coordinate = standard_calc::PointToCoord(cartesian);
  }
}

void HexPlanet::ComputeVertexNeighbours() {
  std::vector<std::unordered_set<HexVertexId>> neighbour_map;
  neighbour_map.reserve(vertices_.size());

  for (size_t i = 0; i < vertices_.size(); i++) {
    neighbour_map.push_back(std::unordered_set<HexVertexId>(HexVertex::kMaxHexVertexNeighbourCount));
  }

  for (const auto &triangle : triangles_) {
    // Neighbours of a
    neighbour_map[triangle.vertex_a].insert(triangle.vertex_b);
    neighbour_map[triangle.vertex_a].insert(triangle.vertex_c);
    // Neighbours of b
    neighbour_map[triangle.vertex_b].insert(triangle.vertex_a);
    neighbour_map[triangle.vertex_b].insert(triangle.vertex_c);
    // Neighbours of c
    neighbour_map[triangle.vertex_c].insert(triangle.vertex_a);
    neighbour_map[triangle.vertex_c].insert(triangle.vertex_b);
  }

  for (HexVertexId i = 0; i < neighbour_map.size(); i++) {
    // There mustn't be more than 6 neighbours for any vertex.
    if (neighbour_map[i].size() > HexVertex::kMaxHexVertexNeighbourCount) {
      throw std::runtime_error("There must not be more than 6 neighbours for any vertex.");
    }

    HexVertexId j = 0;

    // Populate the neighbours array with the candidates.
    for (HexVertexId c : neighbour_map[i]) {
      vertices_[i].neighbours[j] = c;
      j++;
    }

    // Note the number of neighbours for later convenience.
    vertices_[i].neighbour_count = j;

    // Set the rest of the neighbour array to the default (kInvalidHexVertexId).
    for (; j < HexVertex::kMaxHexVertexNeighbourCount; j++) {
      vertices_[i].neighbours[j] = kInvalidHexVertexId;
    }
  }
}

void HexPlanet::ComputeIndirectVertexNeighbours(uint8_t depth) {
  // TODO(areksredzki): Multithread!
  for (HexVertexId id = 0; id < vertices_.size(); id++) {
    HexVertex &vertex = vertices_[id];

    std::unordered_set<HexVertexId> neighbour_map;

    // Allocate enough space for the max number of entries that could be necessary to avoid rehashing.
    neighbour_map.reserve(kDefaultIndirectNeighbourMapSize);

    // Add the subject HexVertex itself.
    neighbour_map.insert(id);

    // Add the direct neighbours
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      neighbour_map.insert(vertex.neighbours[i]);
    }

    // If a neighbour doesn't exist in the set, add it to vertex.indirect_neighbours.
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      ComputeIndirectVertexNeighbourHelper(vertex, neighbour_map, vertex.neighbours[i], depth);
    }
  }
}

void HexPlanet::ComputeIndirectVertexNeighbourHelper(HexVertex &vertex,
                                                     std::unordered_set<HexVertexId> &neighbour_map,
                                                     HexVertexId parent_id,
                                                     uint8_t depth) {
  if (depth == 0) {
    return;
  }

  HexVertex &neighbour = vertices_[parent_id];
  for (size_t i = 0; i < neighbour.neighbour_count; i++) {
    HexVertexId indirect_neighbour_candidate = neighbour.neighbours[i];
    // If a neighbour candidate hasn't been seen before (isn't in the neighbour_map), then add it to
    // vertex.indirect_neighbours.
    if (neighbour_map.find(indirect_neighbour_candidate) == neighbour_map.end()) {
      neighbour_map.insert(indirect_neighbour_candidate);
      vertex.indirect_neighbours.push_back(indirect_neighbour_candidate);
    }

    // Recurse to the next level
    ComputeIndirectVertexNeighbourHelper(vertex,
                                         neighbour_map,
                                         indirect_neighbour_candidate,
                                         static_cast<uint8_t> (depth - 1));
  }
}

void HexPlanet::ComputeVertexNeighbourDistances() {
  for (HexVertex &vertex : vertices_) {
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = vertex.neighbours[i];
      const HexVertex &target = vertices_[neighbour_id];
      vertex.neighbour_distances[i] = standard_calc::DistBetweenTwoCoords(vertex.coordinate, target.coordinate);
    }
  }
}
