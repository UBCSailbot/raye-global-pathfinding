// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXTILE_H
#define DATATYPES_HEXTILE_H

#include <Eigen/Dense>

// A Hextile is a single hex (or sometimes pentagon)
// in the hex tiled planet. It is a single vertex of
// the dual mesh.
struct HexTile {
  HexTile(Eigen::Vector3f p);

  Eigen::Vector3f normal() const;

  // the position of the vert in the tile,
  // and the center of the hex in space
  Eigen::Vector3f m_vertPos;
};

#endif  // DATATYPES_HEXTILE_H
