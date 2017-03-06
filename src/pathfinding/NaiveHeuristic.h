// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_NAIVEHEURISTIC_H_
#define PATHFINDING_NAIVEHEURISTIC_H_

#include "Heuristic.h"

class NaiveHeuristic : public Heuristic {
 public:
  /**
   * @param planet The planet used for heuristic calculations. Note: Not used by this heuristic.
   * @param cost The cost that will always be returned.
   */
  explicit NaiveHeuristic(HexPlanet &planet, uint32_t cost = 1);

  /**
   * Note: This heuristic doesn't actually use the source & target IDs.
   * @param source Source vertex ID
   * @param target Target vertex ID
   * @return The constant cost defined on construction.
   */
  uint32_t calculate(HexVertexId source, HexVertexId target) const override;

 private:
  uint32_t cost_;
};

#endif  // PATHFINDING_NAIVEHEURISTIC_H_
