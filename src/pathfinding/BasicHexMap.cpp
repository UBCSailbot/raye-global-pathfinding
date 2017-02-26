// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicHexMap.h"
#include <ctime>
#include <iostream>

const uint32_t BasicHexMap::kMaxVertexRisk = 10;

BasicHexMap::BasicHexMap(const HexPlanet &planet) : planet_(planet) {
  size_t num_vertices = planet.vertex_count();

  std::srand(static_cast<uint>(std::time(0)));

  for (size_t i = 0; i < num_vertices; ++i) {
    risks_.push_back((std::rand() % kMaxVertexRisk) + 1);
  }
}

uint32_t BasicHexMap::get_risk(HexVertexId vertex_id) {
  if (vertex_id > planet_.vertex_count()) {
    throw std::runtime_error("Vertex does not exist on the planet.");
  }

  return risks_[vertex_id];
}
