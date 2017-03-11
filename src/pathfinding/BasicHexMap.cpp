// Copyright 2017 UBC Sailbot

#include "pathfinding/BasicHexMap.h"

#include <ctime>

constexpr uint32_t BasicHexMap::kDefaultRisk = 0;
constexpr uint32_t BasicHexMap::kDefaultMaxRisk = 10;

BasicHexMap::BasicHexMap(const HexPlanet &planet,
                         const std::vector<uint32_t> &risks,
                         uint32_t default_risk) : planet_(planet), risks_(risks) {
  size_t num_vertices = planet_.vertex_count();

  for (size_t i = risks_.size(); i < num_vertices; ++i) {
    risks_.push_back(default_risk);
  }
}

uint32_t BasicHexMap::get_risk(HexVertexId vertex_id) {
  if (vertex_id >= planet_.vertex_count()) {
    throw std::runtime_error("Vertex does not exist on the planet.");
  }

  return risks_[vertex_id];
}

BasicHexMap BasicHexMap::MakeRandom(const HexPlanet &planet, uint32_t min_risk, uint32_t max_risk) {
  size_t num_vertices = planet.vertex_count();

  std::vector<uint32_t> risks;

  std::srand(static_cast<uint>(std::time(0)));

  for (size_t i = 0; i < num_vertices; ++i) {
    risks.push_back(std::rand() % (max_risk - min_risk + 1) + min_risk);
  }

  return BasicHexMap(planet, risks, 0);
}
