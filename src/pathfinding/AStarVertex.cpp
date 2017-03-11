// Copyright 2017 UBC Sailbot

#include "pathfinding/AStarVertex.h"

AStarVertex::AStarVertex(HexVertexId hex_vertex_id,
                         uint32_t time,
                         uint32_t cost)
    : id_time_index_({hex_vertex_id, time}), cost_(cost) {}
