// Copyright 2017 UBC Sailbot

#include "NaiveCostCalculator.h"

NaiveCostCalculator::NaiveCostCalculator(const HexPlanet &planet) : CostCalculator(planet) {}

uint32_t NaiveCostCalculator::calculate(const HexVertexId source, const HexVertexId target, uint32_t time) const {
  return 1;
}
