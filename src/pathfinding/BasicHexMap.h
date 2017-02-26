// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICHEXMAP_H_
#define PATHFINDING_BASICHEXMAP_H_

#include "planet/HexPlanet.h"

class BasicHexMap {
 public:
  static const uint32_t kMaxVertexRisk;

  /**
   * Creates a BasicHexMap that initializes and stores a risk value for each
   * vertex of a HexPlanet.
   * @param planet The planet.
   */
  explicit BasicHexMap(const HexPlanet &planet);

  /**
   * Gets the risk associated with a specific vertex.
   * @param vertex_id The id of the vertex.
   * @throw std::runtime_error If vertex_id is invalid.
   * @return The risk associated with that vertex.
   */
  uint32_t get_risk(HexVertexId vertex_id);

 private:
  std::vector<uint32_t> risks_;
  HexPlanet planet_;
};

#endif  // PATHFINDING_BASICHEXMAP_H_
