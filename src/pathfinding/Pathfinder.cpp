// Copyright 2017 UBC Sailbot

#include "Pathfinder.h"

Pathfinder::Pathfinder(HexPlanet &planet,
                       const Heuristic &heuristic,
                       const CostCalculator &cost_calculator,
                       HexVertexId start,
                       HexVertexId target)
    : planet_(planet),
      heuristic_(heuristic),
      cost_calculator_(cost_calculator),
      start_(start),
      target_(target) {
  if (start_ >= planet_.vertex_count()) {
    throw std::runtime_error("Start is not a valid vertex.");
  }
  if (target_ >= planet_.vertex_count()) {
    throw std::runtime_error("Target is not a valid vertex.");
  }
}
