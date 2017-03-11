// Copyright 2017 UBC Sailbot

#include "pathfinding/NaiveHeuristic.h"

NaiveHeuristic::NaiveHeuristic(HexPlanet &planet, uint32_t cost)
    : Heuristic(planet), cost_(cost) {}

uint32_t NaiveHeuristic::calculate(HexVertexId source, HexVertexId target) const {
  if (source == target) {
    return 0;
  }

  return cost_;
}
