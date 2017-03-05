// Copyright 2017 UBC Sailbot

#ifndef COMMON_GENERALDEFS_H_
#define COMMON_GENERALDEFS_H_

#include <cstdint>

namespace sailbot {

/**
 * The mean radius of the Earth in meters
 */
constexpr int kEarthRadius = 6371000;

/**
 * The tolerance value used to check if two cartesian coordinates are "almost equal"
 * Units: dimensionless; used on normalized vectors
 */
static constexpr float kPointTolerance = 0.01;

}  // namespace sailbot

#endif  // COMMON_GENERALDEFS_H_
