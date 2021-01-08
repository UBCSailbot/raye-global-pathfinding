// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarPathfinder.h"
#include "common/ProgressBar.h"

#include <memory>
#include <iostream>

AStarPathfinder::AStarPathfinder(HexPlanet &planet,
                                 const Heuristic &heuristic,
                                 const CostCalculator &cost_calculator,
                                 HexVertexId start,
                                 HexVertexId target,
                                 bool use_indirect_neighbours)
    : Pathfinder(planet, heuristic, cost_calculator, start, target), use_indirect_neighbours_(use_indirect_neighbours) {
  if (use_indirect_neighbours_ && !cost_calculator_.is_indirect_neighbour_safe()) {
    throw std::runtime_error("This cost calculator cannot be safely used with indirect neighbours");
  }
}

Pathfinder::Result AStarPathfinder::Run() {
  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<AStarVertex>> open_set;
  TimeIndexValueMap visited;
  if (planet_.subdivision_level() >= kClosedSetReservePlanetSize) {
    visited.reserve(kClosedSetReserveSize);
  }

  // Add start state.
  open_set.emplace(start_, 0, heuristic_.calculate(start_, target_));
  visited[std::make_pair(start_, 0)] = VisitedStateData{0, std::make_pair(kInvalidHexVertexId, 0)};

  const uint32_t max_h_cost = heuristic_.calculate(start_, target_);
  uint32_t min_h_cost = max_h_cost;
  ProgressBar progress_bar;
  int progressCount = 0;

  // TODO(areksredzki): There is currently no check to see that the location is at all reachable.
  // Since there are no bounds on the time dimension, the pathfinder will run forever.
  while (!open_set.empty()) {
    AStarVertex current = open_set.top();
    open_set.pop();

    // The best data for this IdTimeIndex up until now.
    VisitedStateData current_data = visited[current.id_time_index()];

    // Show on progress bar the closest we have gotten to goal
    const uint32_t h_cost = heuristic_.calculate(current.hex_vertex_id(), target_);
    min_h_cost = std::min(min_h_cost, h_cost);

    if (progressCount > 10000) {
      const double progress = 1.0 - static_cast<double>(min_h_cost) / max_h_cost;
      progress_bar.update(progress);
      const std::string text_after_progress_bar = "| Path cost = " + std::to_string(current.cost());
      progress_bar.print(text_after_progress_bar);
      progressCount = 0;
    }

    progressCount++;

    if (current.hex_vertex_id() == target_) {
      // Flush progress bar
      progress_bar.flush();

      stats_.closed_set_size = visited.size();
      stats_.open_set_size = open_set.size();
      return {ConstructPath(current.id_time_index(), visited), current_data.cost, current.time()};
    }

    const HexVertex &vertex = planet_.vertex(current.hex_vertex_id());

    // Process edges to direct neighbours
    for (size_t i = 0; i < vertex.neighbour_count; i++) {
      HexVertexId neighbour_id = vertex.neighbours[i];

      // Calculate the cost and time between the current vertex and this neighbour.
      auto cost_time = cost_calculator_.calculate_neighbour(current.hex_vertex_id(), i, current.time());

      // Total cost from the start to this neighbour.
      uint32_t neighbour_cost = current_data.cost + cost_time.cost;

      // Heuristic cost from this neighbour to the target.
      uint32_t heuristic_cost = heuristic_.calculate(neighbour_id, target_);

      AStarVertex::IdTimeIndex neighbour_id_time_index(neighbour_id, cost_time.time);
      AddNeighbour(open_set, visited, current.id_time_index(), neighbour_id_time_index, neighbour_cost, heuristic_cost);
    }

    if (use_indirect_neighbours_) {
      // Process edges to indirect neighbours
      for (HexVertexId neighbour_id : vertex.indirect_neighbours) {
        // Calculate the cost and time between the current vertex and this neighbour.
        auto cost_time = cost_calculator_.calculate_target(current.hex_vertex_id(), neighbour_id, current.time());

        // Total cost from the start to this neighbour.
        uint32_t neighbour_cost = current_data.cost + cost_time.cost;

        // Heuristic cost from this neighbour to the target.
        uint32_t heuristic_cost = heuristic_.calculate(neighbour_id, target_);

        AStarVertex::IdTimeIndex neighbour_id_time_index(neighbour_id, cost_time.time);
        AddNeighbour(open_set, visited, current.id_time_index(), neighbour_id_time_index, neighbour_cost,
                     heuristic_cost);
      }
    }
  }

  // Should be the total number of nodes in the graph (currently infinite).
  stats_.closed_set_size = visited.size();
  // Should be 0.
  stats_.open_set_size = open_set.size();
  return {{}, 0, 0};
}

void AStarPathfinder::AddNeighbour(VertexQueue &open_set,
                                   TimeIndexValueMap &visited,
                                   const AStarVertex::IdTimeIndex &current_id_time_index,
                                   const AStarVertex::IdTimeIndex &neighbour_id_time_index,
                                   uint32_t neighbour_cost,
                                   uint32_t heuristic_cost) {
  auto item = visited.find(neighbour_id_time_index);

  if (item == visited.end() || neighbour_cost < item->second.cost) {
    if (item == visited.end()) {
      // Create the VisitedData instance.
      visited[neighbour_id_time_index] = VisitedStateData{neighbour_cost, current_id_time_index};
    } else {
      // Update the members of the existing VisitedData instance.
      item->second = {neighbour_cost, current_id_time_index};
    }

    // Add the neighbour to the open set in place (no copy/move operations apart from shuffling the priority_queue).
    open_set.emplace(neighbour_id_time_index, neighbour_cost + heuristic_cost);
  }
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
