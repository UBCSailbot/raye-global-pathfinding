// Copyright 2017 UBC Sailbot

#include "HaversineHeuristic.h"

#include <cmath>
#include <logic/StandardCalc.h>

#include "common/GeneralDefs.h"

HaversineHeuristic::HaversineHeuristic() { }

uint32_t HaversineHeuristic::calculate(const GPSCoordinateFast &source, const GPSCoordinateFast &target) {
  return standard_calc::DistBetweenTwoCoords(source, target);
}
