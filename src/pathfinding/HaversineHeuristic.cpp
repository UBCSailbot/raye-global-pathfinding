// Copyright 2017 UBC Sailbot

#include "HaversineHeuristic.h"

#include <logic/StandardCalc.h>

HaversineHeuristic::HaversineHeuristic(HexPlanet &planet) : Heuristic(planet) {}

uint32_t HaversineHeuristic::calculate(HexVertexId source, HexVertexId target) const {
  return planet_.DistanceBetweenVertices(source, target);
}
