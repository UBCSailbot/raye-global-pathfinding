// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_WEATHERCOSTCALCULATOR_H_
#define PATHFINDING_WEATHERCOSTCALCULATOR_H_

#include "pathfinding/WeatherHexMap.h"
#include "pathfinding/HaversineCostCalculator.h"

class WeatherCostCalculator : public HaversineCostCalculator {
 public:
  /**
   * Creates a WeatherCostCalculator instance that gets the cost from one point
   * to another. The Calculator will take ownership of the map it is given and
   * handle its deletion.
   * @param planet The planet.
   * @param map The risk map for the planet.
   */
  explicit WeatherCostCalculator(HexPlanet &planet, std::unique_ptr<WeatherHexMap> &map);

  /**
   * Calculate the cost to an immediate neighbour of |source| using the
   * Haversine formula and weather data.
   * @param source Source hex vertex ID.
   * @param neighbour Target hex vertex's position in |source|'s neighbour array.
   * @param start_time Starting time step.
   * @throw std::runtime_error |neighbour| is invalid.
   * @return The cost (distance in meters + WeatherHexMap based cost) and ending
   *    time step for an edge.
   */
  Result calculate_neighbour(HexVertexId source, size_t neighbour, uint32_t start_time) const override;

  /**
   * Computes the a cost between two points using the Haversine formula and the
   * WeatherHexMap.
   * Note: Currently just increments time by one.
   * @warning Use calculate_neighbour() if possible!
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param time Starting time step.
   * @throw std::runtime_error If target or source does not exist on the planet.
   * @return The cost (distance in meters + WeatherHexMap based cost) and ending
   *    time step for an edge.
   */
  Result calculate_target(HexVertexId source, HexVertexId target, uint32_t start_time) const override;

  // Class can't be copied
  //WeatherCostCalculator(const WeatherCostCalculator &) = delete;

  // Class can't be moved
  //WeatherCostCalculator(WeatherCostCalculator &&) = delete;

  // Class can't be copy assigned
  //WeatherCostCalculator &operator=(const WeatherCostCalculator &) = delete;

  // Class can't be move assigned
  //WeatherCostCalculator &operator=(WeatherCostCalculator &&) = delete;

 private:
  std::unique_ptr<WeatherHexMap> map_;
  double calculate_map_cost(HexVertexId target, HexVertexId source, uint32_t start_time) const;
};

#endif  // PATHFINDING_WEATHERCOSTCALCULATOR_H_
