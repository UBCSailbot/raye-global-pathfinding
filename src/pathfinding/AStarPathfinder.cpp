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
  std::priority_queue<AStarVertex> open_set;
  TimeIndexVertexMap closed_set;

  // Add start state.
  open_set.emplace(start_, 0, 0, heuristic_.calculate(start_, target_), std::make_pair(kInvalidHexVertexId, 0));

  // TODO(areksredzki): There is currently no check to see that the location is at all reachable.
  // Since there are no bounds on the time dimension, the pathfinder will run forever.
  while (!open_set.empty()) {
    AStarVertex current = open_set.top();
    open_set.pop();

    const AStarVertex::IdTimeIndex &current_id_time_index = current.id_time_index();

    if (closed_set.find(current_id_time_index) != closed_set.end()) {
      // This IdTimeIndex has already been visited (with a lower cost) so continue to the next one.
      continue;
    }

    // Add to closed set.
    closed_set.insert({current_id_time_index, current});

    if (current.hex_vertex_id() == target_) {
      stats_.closed_set_size = closed_set.size();
      stats_.open_set_size = open_set.size();
      return {ConstructPath(current, closed_set), current.cost(), current.time()};
    }

    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = vertex.neighbours[i];

      // Calculate the cost and time between the current vertex and this neighbour.
      auto cost_time = cost_calculator_.calculate(current.hex_vertex_id(), neighbour_id, current.time());
      // Total cost from the start to this neighbour.
      double neighbour_cost = current.cost() + cost_time.cost;

      // Heuristic cost from this neighbour to the target.
      uint32_t heuristic_cost = heuristic_.calculate(neighbour_id, target_);

      // Add the neighbour to the open set in place (no copy/move operations apart from shuffling the priority_queue).
      open_set.emplace(neighbour_id, cost_time.time, neighbour_cost, heuristic_cost, current_id_time_index);
    }
  }

  // Should be the total number of nodes in the graph (currently infinite).
  stats_.closed_set_size = closed_set.size();
  // Should be 0.
  stats_.open_set_size = open_set.size();
  return {{}, 0, 0};
}

std::vector<HexVertexId> AStarPathfinder::ConstructPath(AStarVertex vertex, const TimeIndexVertexMap &closed_set) {
  auto path = std::deque<HexVertexId>();

  auto it = closed_set.find(vertex.id_time_index());

  while (it != closed_set.end()) {
    vertex = it->second;
    path.push_front(vertex.hex_vertex_id());

    if (vertex.hex_vertex_id() == start_) {
      break;
    }

    it  = closed_set.find(vertex.parent());
  }

  return {path.begin(), path.end()};
}

