// Copyright 2017 UBC Sailbot

#include "HexTile.h"

HexTile::HexTile(Eigen::Vector3f p) : m_vertPos(p) {}

Eigen::Vector3f HexTile::normal() const {
  return m_vertPos.normalized();
}