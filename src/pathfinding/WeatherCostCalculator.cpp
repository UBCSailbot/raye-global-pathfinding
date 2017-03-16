// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherCostCalculator.h"

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

  result.cost += calculate_map_cost(source, target, start_time);

  return result;
}

CostCalculator::Result WeatherCostCalculator::calculate_target(HexVertexId source,
                                                             HexVertexId target,
                                                             uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_target(source, target, start_time);

  result.cost += calculate_map_cost(source, target, start_time);

  return result;
}

double WeatherCostCalculator::calculate_map_cost(HexVertexId target,
                                               HexVertexId source,
                                               uint32_t time) const {
  return map_->get_weather(source, time).wave_height
         + map_->get_weather(target, time).wave_height;
}
