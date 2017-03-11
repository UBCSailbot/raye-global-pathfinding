// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_COSTCALCULATOR_H_
#define PATHFINDING_COSTCALCULATOR_H_

#include <cstdint>

#include "planet/HexPlanet.h"
#include "datatypes/HexDefs.h"

class CostCalculator {
 public:
  struct Result {
    /// The computed edge cost.
    uint32_t cost;
    /// The ending time step.
    uint32_t time;
  };

  explicit CostCalculator(HexPlanet &planet) : planet_(planet) {}

  /**
   * Calculate the cost to an immediate neighbour of |source|.
   * @param source Source hex vertex ID.
   * @param neighbour Target hex vertex's position in |source|'s neighbour array.
   * @param start_time Starting time step.
   * @throw std::runtime_error |neighbour| is invalid.
   * @return The cost and ending time step for an edge.
   */
  virtual Result calculate_neighbour(HexVertexId source, size_t neighbour, uint32_t start_time) const {
    const HexVertex &source_vertex = planet_.vertex(source);
    if (neighbour >= source_vertex.neighbour_count) {
      throw std::runtime_error("Calculating cost to invalid neighbour");
    }
    HexVertexId target = source_vertex.neighbours[neighbour];
    return calculate_target(source, target, start_time);
  }

  /**
   * @param source Source hex vertex ID.
   * @param target Target hex vertex ID.
   * @param start_time Starting time step.
   * @return The cost and ending time step for an edge.
   */
  virtual Result calculate_target(HexVertexId source, HexVertexId target, uint32_t start_time) const = 0;

  /**
   * @return Whether this cost calculator is safe for usage with indirect neighbours.
   */
  virtual bool is_indirect_neighbour_safe() const { return false; }

 protected:
  HexPlanet &planet_;
};

#endif  // PATHFINDING_COSTCALCULATOR_H_
