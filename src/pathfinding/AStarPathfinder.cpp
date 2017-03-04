// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarPathfinder.h"

#include <memory>
#include <queue>

AStarPathfinder::AStarPathfinder(const HexPlanet &planet,
                                 Heuristic *heuristic,
                                 CostCalculator *cost_calculator,
                                 HexVertexId start,
                                 HexVertexId target)
    : Pathfinder(planet, heuristic, cost_calculator, start, target) {}

std::vector<HexVertexId> AStarPathfinder::Run() {
  std::priority_queue<AStarVertex, std::vector<AStarVertex>> open_set;

  AStarVertex start_vertex(start_, 0, 0);
  open_set.push(start_vertex);

  closed_set.insert({start_vertex.id_time_coordinate(), start_vertex});

  while (!open_set.empty()) {
    auto current = open_set.top();
    open_set.pop();

    if (current.hex_vertex_id() == target_) {
      return ConstructPath(current);
    }

    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());
    const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = neighbours[i];
      double new_cost = current.cost() + cost_calculator_->
          calculate(neighbour_id, current.hex_vertex_id(), current.time());
      uint32_t new_time = current.time() + 1;
      if (!closed_set.count({neighbour_id, new_time})
          || new_cost < closed_set.find({neighbour_id, new_time})->second.cost()) {
        double cost = new_cost + heuristic_->
            calculate(planet_.GPSCoordinateFromHexVertex(neighbour_id), planet_.GPSCoordinateFromHexVertex(target_));

        AStarVertex neighbour_node = AStarVertex(neighbour_id, new_time, cost);
        neighbour_node.set_parent(current.id_time_coordinate());

        open_set.push(neighbour_node);
        closed_set.insert({neighbour_node.id_time_coordinate(), neighbour_node});
      }
    }
  }

  return std::vector<HexVertexId>();
}

std::vector<HexVertexId> AStarPathfinder::ConstructPath(AStarVertex vertex) {
  auto path = std::deque<HexVertexId>();

  while (closed_set.count(vertex.id_time_coordinate())) {
    path.push_front(vertex.hex_vertex_id());
    if (vertex.hex_vertex_id() == start_) {
      break;
    }
    vertex = closed_set.find(vertex.parent())->second;
  }

  return {path.begin(), path.end()};
}

