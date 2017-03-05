// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_NAIVEHEURISTIC_H_
#define PATHFINDING_NAIVEHEURISTIC_H_

#include "Heuristic.h"

class NaiveHeuristic : public Heuristic {
 public:
  explicit NaiveHeuristic(uint32_t default_cost = 0);

  uint32_t calculate(const GPSCoordinateFast &source, const GPSCoordinateFast &target) const override;

 private:
  uint32_t default_cost_;
};

#endif   // PATHFINDING_NAIVEHEURISTIC_H_
