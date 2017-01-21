// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_HEXTRI_H
#define DATATYPES_HEXTRI_H

#include <Eigen/Dense>
#include "HexTile.h"

#include <cstdint>
#include <vector>

// A hextri is an element of the dual
// of the (mostly) hex tiling
struct HexTri {
  HexTri(uint32_t a, uint32_t b, uint32_t c);

  // indices into the hex list
  uint32_t m_hexA, m_hexB, m_hexC;

  // Returns the center of the tri
  Eigen::Vector3f getCenter(const std::vector<HexTile> &hexes);

  // Temporaries that don't need to stick around
  union {
    // Just used in the construction process
    uint32_t m_newvert;

    // angle around tile center
    float m_angle;
  } m_tmp;

  // Dbg color
  //Imath::C4f m_dbgColor;
};

#endif  // DATATYPES_HEXTRI_H
