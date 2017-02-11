// Copyright 2017 UBC Sailbot

#include <iostream>

#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/NaiveCostCalculator.h>
#include <pathfinding/AStarPathfinder.h>

int main(int, char const *[]) {
  HexPlanet planet(0);
  HaversineHeuristic heuristic = HaversineHeuristic();
  NaiveCostCalculator cost_calculator = NaiveCostCalculator(planet);

  AStarPathfinder pathfinder(planet, &heuristic, &cost_calculator, 0, 1);
  auto path = pathfinder.Run();

  std::cout << "Path: " << std::endl;
  for (auto point : path) {
    std::cout << point << ", " << std::endl;
  }

  std::cout << std::endl;
  return 0;
}
