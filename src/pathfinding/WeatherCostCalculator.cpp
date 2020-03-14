// Copyright 2017 UBC Sailbot

#include "pathfinding/WeatherCostCalculator.h"

#include <iostream>

#define WEATHER_FACTOR 10000    //Produces the best path as of yet, needs more testing

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

  result.cost += WEATHER_FACTOR * calculate_map_cost(source, target, start_time);

  return result;
}

CostCalculator::Result WeatherCostCalculator::calculate_target(HexVertexId source,
                                                             HexVertexId target,
                                                             uint32_t start_time) const {
  Result result = HaversineCostCalculator::calculate_target(source, target, start_time);

//  std::cout << result.cost << " ";

  result.cost += WEATHER_FACTOR * calculate_map_cost(source, target, start_time);

  //std::cout << result.cost << std::endl;

  return result;
}

double WeatherCostCalculator::calculate_map_cost(HexVertexId target,
                                               HexVertexId source,
                                               uint32_t time) const {
  double target_mag = map_->get_weather(target, time).wind_speed,
         source_mag = map_->get_weather(source, time).wind_speed;
  if(target_mag<=5){
    return 50 - (source_mag + target_mag);
  }
  else if(target_mag<18){
    return (source_mag + target_mag) * 2 - 50;
  }
  return 1000; //Really big cost value if wind speed is greater than 40 kts
}
