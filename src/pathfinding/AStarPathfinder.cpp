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
  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<AStarVertex>> open_set;
  TimeIndexValueMap visited;
  if (planet_.subdivision_level() >= kClosedSetReservePlanetSize) {
    visited.reserve(kClosedSetReserveSize);
  }

  // Add start state.
  open_set.emplace(start_, 0, heuristic_.calculate(start_, target_));
  visited[std::make_pair(start_, 0)] = VisitedStateData{0, std::make_pair(kInvalidHexVertexId, 0)};

  // TODO(areksredzki): There is currently no check to see that the location is at all reachable.
  // Since there are no bounds on the time dimension, the pathfinder will run forever.
  while (!open_set.empty()) {
    AStarVertex current = open_set.top();
    open_set.pop();

    if (current.hex_vertex_id() == target_) {
      stats_.closed_set_size = visited.size();
      stats_.open_set_size = open_set.size();
      return {ConstructPath(current.id_time_index(), visited), current.cost(), current.time()};
    }

    // The best data for this IdTimeIndex up util now.
    VisitedStateData current_data = visited[current.id_time_index()];

    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = vertex.neighbours[i];

      // Calculate the cost and time between the current vertex and this neighbour.
      auto cost_time = cost_calculator_.calculate_neighbour(current.hex_vertex_id(), i, current.time());

      // Total cost from the start to this neighbour.
      uint32_t neighbour_cost = current_data.cost + cost_time.cost;

      // Heuristic cost from this neighbour to the target.
      uint32_t heuristic_cost = heuristic_.calculate(neighbour_id, target_);

      AStarVertex::IdTimeIndex neighbour_id_time_index(neighbour_id, cost_time.time);
      auto search = visited.find(neighbour_id_time_index);

      if (search == visited.end() || neighbour_cost < search->second.cost) {
        if (search == visited.end()) {
          // Create the VisitedData instance.
          visited[neighbour_id_time_index] = VisitedStateData{neighbour_cost, current.id_time_index()};
        } else {
          // Update the members of the existing VisitedData instance.
          search->second = {neighbour_cost, current.id_time_index()};
        }

        // Add the neighbour to the open set in place (no copy/move operations apart from shuffling the priority_queue).
        open_set.emplace(neighbour_id, cost_time.time, neighbour_cost + heuristic_cost);
      }
    }
  }

  // Should be the total number of nodes in the graph (currently infinite).
  stats_.closed_set_size = visited.size();
  // Should be 0.
  stats_.open_set_size = open_set.size();
  return {{}, 0, 0};
}

std::vector<HexVertexId> AStarPathfinder::ConstructPath(AStarVertex::IdTimeIndex vertex,
                                                        const TimeIndexValueMap &visited) {
  auto path = std::deque<HexVertexId>();

  auto it = visited.find(vertex);

  while (it != visited.end()) {
    vertex = it->first;
    path.push_front(vertex.first);

    if (vertex.first == start_) {
      break;
    }

    it = visited.find(it->second.parent);
  }

  return {path.begin(), path.end()};
}

