// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarPathfinder.h"

#include <memory>
#include <queue>

AStarPathfinder::AStarPathfinder(HexPlanet &planet,
                                 const Heuristic &heuristic,
                                 const CostCalculator &cost_calculator,
                                 HexVertexId start,
                                 HexVertexId target)
    : Pathfinder(planet, heuristic, cost_calculator, start, target) {}

Pathfinder::Result AStarPathfinder::Run() {
  std::priority_queue<AStarVertex, std::vector<AStarVertex>> open_set;

  AStarVertex start_vertex(start_, 0, 0, heuristic_.calculate(start_, target_));
  open_set.push(start_vertex);

  closed_set.insert({start_vertex.id_time_index(), start_vertex});

  while (!open_set.empty()) {
    auto current = open_set.top();
    open_set.pop();

    if (current.hex_vertex_id() == target_) {
      return {ConstructPath(current), current.cost(), current.time()};
    }

    const AStarVertex::IdTimeIndex &current_id_time_index = current.id_time_index();
    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());
    const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = neighbours[i];
      auto cost_time = cost_calculator_.calculate(current.hex_vertex_id(), neighbour_id, current.time());
      double new_cost = current.cost() + cost_time.cost;

      AStarVertex::IdTimeIndex neighbour_id_time_index(neighbour_id, cost_time.time);
      bool already_visited = closed_set.count(neighbour_id_time_index) > 0;

      if (!already_visited || new_cost < closed_set.find(neighbour_id_time_index)->second.cost()) {
        uint32_t heuristic_cost = heuristic_.calculate(neighbour_id, target_);

        AStarVertex neighbour_node = AStarVertex(neighbour_id, cost_time.time, new_cost, heuristic_cost);
        neighbour_node.set_parent(current_id_time_index);

        open_set.push(neighbour_node);
        if (already_visited) {
          closed_set.erase(neighbour_id_time_index);
        }
        closed_set.insert({neighbour_id_time_index, neighbour_node});
      }
    }
  }

  return {{}, 0, 0};
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

