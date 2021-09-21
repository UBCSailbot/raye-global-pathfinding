// Copyright 2017 UBC Sailbot

#include "datatypes/HexVertex.h"

HexVertex::HexVertex(Eigen::Vector3f p) : vertex_position(p) {}
HexVertex::HexVertex(Eigen::Vector3f vertex_position__, GPSCoordinateFast coordinate__, std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbours__, std::array<HexVertexId, kMaxHexVertexNeighbourCount> neighbour_distances__, HexVertexId neighbour_count__, std::vector<HexVertexId> indirect_neighbours__) : coordinate(coordinate__), vertex_position(vertex_position__), neighbours(neighbours__), neighbour_distances(neighbour_distances__), neighbour_count(neighbour_count__), indirect_neighbours(indirect_neighbours__) {}
