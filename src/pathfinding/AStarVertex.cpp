// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarVertex.h"

AStarVertex::AStarVertex(HexVertexId hex_vertex_id,
                         uint32_t time,
                         double cost,
                         double heuristic_cost,
                         IdTimeIndex parent)
    : id_time_index_({hex_vertex_id, time}), cost_(cost), heuristic_cost_(heuristic_cost), parent_(parent) {}
