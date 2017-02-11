// Copyright 2017 UBC Sailbot

#include "NaiveCostCalculator.h"

NaiveCostCalculator::NaiveCostCalculator(const HexPlanet &planet) : CostCalculator(planet) {}

uint32_t NaiveCostCalculator::calculate(HexVertexId source, HexVertexId target, uint32_t time) {
  return 1;
}
