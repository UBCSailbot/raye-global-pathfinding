// Copyright 2016 UBC Sailbot

#ifndef LOGIC_STANDARDCALC_H_
#define LOGIC_STANDARDCALC_H_

#include <Eigen/Dense>

#include "common/GeneralDefs.h"
#include "datatypes/GPSCoordinateFast.h"

namespace standard_calc {

/**
 * Outputs 3d point given the Coordinate (i.e. longitude, latitude)
 * @param coord
 * @param surface_position
 * @return Whether on surface or not
 */
void CoordToPoint(const GPSCoordinateFast &coord, Eigen::Vector3f *surface_position, double r = 1.0);

/**
 * Computes the GPS Coordinate (i.e. longitude, latitude) given 3D point.
 * @param surface_position A vector that describes a point of the planet's surface.
 * @throws std::runtime_error If the surface_position isn't on the surface of the planet (within POINT_TOLERANCE).
 * @return The GPS Coordinate of the surface position.
 */
GPSCoordinateFast PointToCoord(const Eigen::Vector3f &surface_position);

/**
 * Returns true or false depending on weather the point is on the sphere of given radius
 * @param point (xyz cartesian coordinates) and R (radius of sphere)
 * @return true (on sphere) or false (not on sphere)
 */
bool is_on_sphere(const Eigen::Vector3f &point, const double R = 1.0);

/**
 * Calculates the "as the bird flies" distance between to coordinates using the Haversine formula.
 * @param coord1 Coordinate 1.
 * @param coord2 Coordinate 2.
 * @return Distance between the two coordinates in meters.
 */
uint32_t DistBetweenTwoCoords(const GPSCoordinateFast &coord1, const GPSCoordinateFast &coord2);

/**
 * Calculate the bearing required to get from |origin| to |dest|.
 * Note: If |origin| and |dest| are the same coordinate, 0 is returned.
 * @param origin Origin position.
 * @param dest Destination position.
 * @return Bearing to dest from origin in degrees, relative to North. Positive angles rotate clockwise from North.
 */
double BearingBetweenTwoCoords(const GPSCoordinateFast &origin, const GPSCoordinateFast &dest);

/**
 * Converts a vector to degrees with respect to North (up).
 * @param x Horizontal delta, positive North (up).
 * @param y Vertical delta, positive East (right).
 * @return Angle formed by |x| and |y| in degrees with respect to North (up).
 */
double VectorToDegrees(double x, double y);

/**
 * @param angle Input angle in radians.
 * @return |angle| bounded to PI radians.
 */
double BoundToPI(double angle);

/**
 * Bounds the provided angle between (-180, 180] degrees.
 * Ex. 360 becomes 0, 270 becomes -90, -450 becomes -90.
 * @param angle Input angle in degrees.
 * @return The bounded angle in degrees.
 */
double BoundTo180(double angle);

/**
 * Bounds the provided angle between (-180, 180] degrees.
 * Ex. 360 becomes 0, 270 becomes -90, -450 becomes -90.
 * @param angle Input angle in degrees.
 * @return The bounded angle in degrees.
 */
int16_t BoundTo180Exact(int16_t angle);

/**
 * Bounds the provided angle between (-180, 180] degrees * |multiplier|.
 * Ex. With a multiplier of 10, 3600 becomes 0, 2700 becomes -900, -4500 becomes -900.
 * @param angle Input angle in degrees * |multiplier|.
 * @param multiplier The exponent muliplier for a degree. A single degree is |multiplier|.
 * @return The bounded angle in degrees * |multiplier|.
 */
int32_t BoundTo180Exact(int32_t angle, int32_t multiplier);

/**
 * Determines whether |middle_angle| is in the acute angle between the other two bounding angles.
 * Note: Input angles are bounded to 180 for safety.
 * Ex. -180 is between -90 and 110 but not between -90 and 80.
 * @param first_angle First angle in degrees.
 * @param middle_angle Middle angle in degrees.
 * @param second_angle Second angle in degrees.
 * @return Whether |middle_angle| is between |first_angle| and |second_angle| (exclusive).
 */
bool IsAngleBetween(double first_angle, double middle_angle, double second_angle);

/**
 * Calculate the smallest angle between two angles. Input angles must be between [-180, 180].
 * @param first_angle First angle in degrees.
 * @param second_angle Second angle in degrees.
 * @return The smallest angle between |first_angle| and |second_angle| (signed).
 */
double calculate_angle_delta(double first_angle, double second_angle);

/**
 * Efficiently calculates the power of 10 to |n|.
 * @param n Exponent.
 * @return The power of 10 to |n|.
 */
int32_t Long10Pow(int32_t n);

bool are_equal(double a, double b, double epsilon);
bool are_equal(double a, double b);

/**
 * Checks if other vector is (within absoluteTolerance) equal to the vector object
 * absoluteTolerance = tolerancePercentage * scale (e.g. 1% * 1000m = 10m tolerance)
 * @param other vector
 * @param scale: used to convert percent tolerance to absolute tolerance
 * @return true if equal within tolerance, false otherwise
 */
bool almost_equal(const Eigen::Vector3f &point1,
                  const Eigen::Vector3f &point2,
                  const double tolerance = sailbot::kPointTolerance);

/**
 * Converts degrees to radians.
 * @param degrees Value to convert.
 * @return |degreees| converted to radians.
 */
double deg_to_rad(double degrees);

/**
 * Converts radians to degrees.
 * @param radians Value to convert.
 * @return |radians| converted to degrees.
 */
double rad_to_deg(double radians);

}  // namespace standard_calc

#endif  // LOGIC_STANDARDCALC_H_
