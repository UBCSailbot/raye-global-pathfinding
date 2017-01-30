// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXDEFS_H
#define DATATYPES_HEXDEFS_H

#include <cstdint>
#include <cstddef>

typedef uint32_t HexVertexId;

/// The ID value used for denoting an invalid HexVertexID.
static constexpr HexVertexId kInvalidHexVertexId = static_cast<const HexVertexId> (-1);

enum class StaticVertexType : uint8_t {
  obstacle,  ///< There's an static obstacle (land) at this tile.
  start,  ///< This is the starting point for the search.
  target  ///< This is the target point for the search.
};

struct StaticVertexData {
  StaticVertexType type;
};

struct DynamicVertexData {
  double cost;  ///< Static cost of being at this node.
};

struct DynamicPathfinderVertexData {
  double cost;  ///< The lowest total cost of getting to this node (best path found thus far).
  HexVertexId predecessor = kInvalidHexVertexId;  ///< The id of the predecessor to this node,
};

#endif  // DATATYPES_HEXDEFS_H
