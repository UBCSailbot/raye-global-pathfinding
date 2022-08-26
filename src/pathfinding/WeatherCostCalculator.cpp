// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherCostCalculator.h"
#include "pathfinding/WeatherHexMap.h"

#include <iostream>

WeatherCostCalculator::WeatherCostCalculator(HexPlanet &planet,
                                           std::unique_ptr<WeatherHexMap> &map, int weather_factor)
    : HaversineCostCalculator(planet), map_(std::move(map)), weather_factor_(weather_factor) {}

CostCalculator::Result WeatherCostCalculator::calculate_neighbour(HexVertexId source,
                                                                size_t neighbour,
                                                                uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_neighbour(source, neighbour, start_time);

  // Get neighbour vertex ID
  // |neighbour| is valid, else an exception would have been thrown earlier
  HexVertexId target = planet_.vertex(source).neighbours[neighbour];

  result.cost += weather_factor_ * calculate_map_cost(source, target, start_time);

  return result;
}

CostCalculator::Result WeatherCostCalculator::calculate_target(HexVertexId source,
                                                             HexVertexId target,
                                                             uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_target(source, target, start_time);

  // std::cout << result.cost << " ";

  result.cost += weather_factor_ * calculate_map_cost(source, target, start_time);

  // std::cout << result.cost << std::endl;

  return result;
}

double WeatherCostCalculator::calculate_map_cost(HexVertexId target,
                                               HexVertexId source,
                                               uint32_t time) const {
  double target_mag = map_->get_weather(target, time).wind_speed,
         source_mag = map_->get_weather(source, time).wind_speed,
         mag = (source_mag + target_mag)/2;  // Average of this node and the next

  if (mag <= 4) {   // https://www.desmos.com/calculator/s83nzwulue
    return 15;
  } else if (mag <= 7) {
      return 35 - 5 * mag;
  } else if (mag <= 12) {
      return 0;
  } else if (mag <= 15) {
      return (5 / 3) * mag - 20;
  } else if (mag <= 22) {
      return 5 * mag - 70;
  }
  return 1000;  // Really big cost value if wind speed is greater than 30 kts
}
