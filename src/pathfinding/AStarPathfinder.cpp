// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarPathfinder.h"

AStarPathfinder::AStarPathfinder(const HexPlanet &planet,
                                 Heuristic *heuristic,
                                 CostCalculator *cost_calculator,
                                 HexVertexId start,
                                 HexVertexId target)
    : Pathfinder(planet, heuristic, cost_calculator, start, target) {}

std::vector<HexVertexId> AStarPathfinder::Run() {
  return std::vector<HexVertexId>();
}
