// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICHEXMAP_H_
#define PATHFINDING_BASICHEXMAP_H_

#include "planet/HexPlanet.h"

/**
 * @brief Stores a risk value for each vertex of a HexPlanet.
 */
class BasicHexMap {
 public:
  static const uint32_t kDefaultRisk;
  static const uint32_t kDefaultMaxRisk;

  /**
   * Initializes a risk of value |kDefaultRisk| for all vertices that are not initialized by |risks|.
   * @param planet The planet.
   * @param risks The initial risk map.
   */
  explicit BasicHexMap(const HexPlanet &planet,
                       const std::vector<uint32_t> &risks = {},
                       uint32_t default_risk = kDefaultRisk);

  /**
   * Creates a BasicHexMap with randomly initialized risks between |min_risk| and |max_risk|.
   * @param planet The planet.
   * @param min_risk The minimum risk used for computing the random risks.
   * @param max_risk The maximum risk used for computing the random risks.
   */
  static BasicHexMap MakeRandom(const HexPlanet &planet,
                                uint32_t min_risk = kDefaultRisk,
                                uint32_t max_risk = kDefaultMaxRisk);

  /**
   * Gets the risk associated with a specific vertex.
   * @param vertex_id The id of the vertex.
   * @throw std::runtime_error If vertex_id is invalid.
   * @return The risk associated with that vertex.
   */
  uint32_t get_risk(HexVertexId vertex_id);

 private:
  const HexPlanet &planet_;
  std::vector<uint32_t> risks_;
};

#endif  // PATHFINDING_BASICHEXMAP_H_
