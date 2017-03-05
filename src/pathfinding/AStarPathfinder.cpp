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

  const GPSCoordinateFast &start_GPS_coordinate = planet_.GPSCoordinateFromHexVertex(start_);
  const GPSCoordinateFast &target_GPS_coordinate = planet_.GPSCoordinateFromHexVertex(target_);

  AStarVertex start_vertex(start_, 0, 0, heuristic_->calculate(start_GPS_coordinate, target_GPS_coordinate));
  open_set.push(start_vertex);

  closed_set.insert({start_vertex.id_time_index(), start_vertex});

  while (!open_set.empty()) {
    auto current = open_set.top();
    open_set.pop();

    if (current.hex_vertex_id() == target_) {
      return ConstructPath(current);
    }

    const AStarVertex::IdTimeIndex &current_id_time_index = current.id_time_index();
    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());
    const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = neighbours[i];
      double new_cost = current.cost() + cost_calculator_->
          calculate(current.hex_vertex_id(), neighbour_id, current.time());
      uint32_t new_time = current.time() + 1;
      AStarVertex::IdTimeIndex neighbour_id_time_index(neighbour_id, new_time);
      bool already_visited = closed_set.count(neighbour_id_time_index) > 0;

      if (!already_visited || new_cost < closed_set.find(neighbour_id_time_index)->second.cost()) {
        const GPSCoordinateFast &neighbour_GPS_coordinate = planet_.GPSCoordinateFromHexVertex(neighbour_id);
        uint32_t heuristic_cost = heuristic_->calculate(neighbour_GPS_coordinate, target_GPS_coordinate);

        AStarVertex neighbour_node = AStarVertex(neighbour_id, new_time, new_cost, heuristic_cost);
        neighbour_node.set_parent(current_id_time_index);

        open_set.push(neighbour_node);
        if (already_visited) {
          closed_set.erase(neighbour_id_time_index);
        }
        closed_set.insert({neighbour_id_time_index, neighbour_node});
      }
    }
  }

  return std::vector<HexVertexId>();
}

std::vector<HexVertexId> AStarPathfinder::ConstructPath(AStarVertex vertex) {
  auto path = std::deque<HexVertexId>();

  while (closed_set.count(vertex.id_time_index())) {
    path.push_front(vertex.hex_vertex_id());
    if (vertex.hex_vertex_id() == start_) {
      break;
    }
    vertex = closed_set.find(vertex.parent())->second;
  }

  return {path.begin(), path.end()};
}

