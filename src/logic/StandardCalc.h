// Copyright 2016 UBC Sailbot

#ifndef LOGIC_STANDARDCALC_H_
#define LOGIC_STANDARDCALC_H_

#include <datatypes/GPSCoordinateFast.h>

namespace standard_calc {

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
double AngleBetweenTwoCoords(const GPSCoordinateFast &origin, const GPSCoordinateFast &dest);

/**
 * Calculates the cos law angle using c^2 = a^2 + b^2 - 2*a*b*cos(theta).
 * @param a Side length a.
 * @param b Side length b.
 * @param c Side length c.
 * @return Cos law angle in radians.
 */
double FindCosLawAngle(double a, double b, double c);

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
 * Bounds the provided angle between [-180, 180) degrees.
 * Ex. 360 becomes 0, 270 becomes -90, -450 becomes -90.
 * @param angle Input angle in degrees.
 * @return The bounded angle in degrees.
 */
double BoundTo180(double angle);
/**
 * Bounds the provided angle between [-180, 180) degrees.
 * Ex. 360 becomes 0, 270 becomes -90, -450 becomes -90.
 * @param angle Input angle in degrees.
 * @return The bounded angle in degrees.
 */
int16_t BoundTo180Exact(int16_t angle);
/**
 * Bounds the provided angle between [-180, 180) degrees * |multiplier|.
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
 * calculate_angle_delta
 * Requires: first angle, second angle
 * Returns: the angle delta
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
 * Converts degrees to redians.
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

/**
 * Calculates the output of a low pass filter update.
 * Y(n) = (1-ß)*Y(n-1) + (ß*X(n))) = Y(n-1) - (ß*(Y(n-1)-X(n)))
 *
 * @param current Current value.
 * @param raw Raw value (typically sensor data).
 * @param beta Beta value (0 < B < 1).
 * @return Filtered value.
 */
double low_pass_filter_update(double current, double raw, double beta);

}  // namespace standard_calc

#endif  // LOGIC_STANDARDCALC_H_
