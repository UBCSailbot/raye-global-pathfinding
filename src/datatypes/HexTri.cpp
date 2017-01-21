// Copyright 2017 UBC Sailbot

#include "HexTri.h"

HexTri::HexTri(uint32_t a, uint32_t b, uint32_t c) :
    m_hexA(a), m_hexB(b), m_hexC(c) {
  // Mark newvert as uninitialized
  m_tmp.m_newvert = -1;

  // DBG color. For debugging
  //m_dbgColor.r = (float)rand() / (float)RAND_MAX;
  //m_dbgColor.g = (float)rand() / (float)RAND_MAX;
  //m_dbgColor.b = (float)rand() / (float)RAND_MAX;

}

Eigen::Vector3f HexTri::getCenter(const std::vector<HexTile> &hexes) {
  Eigen::Vector3f pntCenter = hexes[m_hexA].m_vertPos;
  pntCenter += hexes[m_hexB].m_vertPos;
  pntCenter += hexes[m_hexC].m_vertPos;
  pntCenter /= 3.0f;

  return pntCenter;
}