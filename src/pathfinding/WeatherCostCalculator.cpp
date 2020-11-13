// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherCostCalculator.h"
#include "pathfinding/WeatherHexMap.h"

#include <iostream>

WeatherCostCalculator::WeatherCostCalculator(HexPlanet &planet,
                                           std::unique_ptr<WeatherHexMap> &map)
    : HaversineCostCalculator(planet), map_(std::move(map)) {}

CostCalculator::Result WeatherCostCalculator::calculate_neighbour(HexVertexId source,
                                                                size_t neighbour,
                                                                uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_neighbour(source, neighbour, start_time);

  // Get neighbour vertex ID
  // |neighbour| is valid, else an exception would have been thrown earlier
  HexVertexId target = planet_.vertex(source).neighbours[neighbour];

  result.cost += weather_factor * calculate_map_cost(source, target, start_time);

  return result;
}

CostCalculator::Result WeatherCostCalculator::calculate_target(HexVertexId source,
                                                             HexVertexId target,
                                                             uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_target(source, target, start_time);

  std::cout << result.cost << " ";

  result.cost += weather_factor * calculate_map_cost(source, target, start_time);

  std::cout << result.cost << std::endl;

  return result;
}

double WeatherCostCalculator::calculate_map_cost(HexVertexId target,
                                               HexVertexId source,
                                               uint32_t time) const {
  double target_mag = map_->get_weather(target, time).wind_speed,
         source_mag = map_->get_weather(source, time).wind_speed,
         mag = (source_mag + target_mag)/2;  // Average of this node and the next

  if (mag <= 5) {  // See https://www.desmos.com/calculator/md1byjfsl2
    return 17 - mag;
  } else if (mag < 11) {
    return 22 - mag * 2;
  } else if (mag < 16) {
    return 0;
  } else if (mag < 30) {
    return mag * 2 - 32;
  }
  return 1000;  // Really big cost value if wind speed is greater than 30 kts
}
