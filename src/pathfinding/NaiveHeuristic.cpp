// Copyright 2017 UBC Sailbot

#include "NaiveHeuristic.h"

NaiveHeuristic::NaiveHeuristic(uint32_t default_cost) : default_cost_(default_cost) {}

uint32_t NaiveHeuristic::calculate(const GPSCoordinateFast &source, const GPSCoordinateFast &target) const {
  return default_cost_;
}
