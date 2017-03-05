// Copyright 2016 UBC Sailbot

#include "logic/StandardCalc.h"

#include <cmath>

namespace standard_calc {

uint32_t DistBetweenTwoCoords(const GPSCoordinateFast &coord1, const GPSCoordinateFast &coord2) {
  if (coord1 == coord2) {
    return 0;
  }

  double a1, a2, a3, a, c;
  a1 = pow(sin((coord2.latitude() - coord1.latitude()) / 2), 2);
  a2 = cos(coord1.latitude()) * cos(coord2.latitude());
  a3 = pow(sin((coord2.longitude() - coord1.longitude()) / 2), 2);
  a = a1 + (a2 * a3);
  c = 2 * atan2(sqrt(a), sqrt(1 - a));

  // Simply drops the decimal precision. This will always be a lower bound.
  return static_cast<uint32_t> (sailbot::kEarthRadius * c);
}

double BearingBetweenTwoCoords(const GPSCoordinateFast &origin, const GPSCoordinateFast &dest) {
  if (origin == dest) {
    return 0;
  }

  double a = sin(dest.longitude() - origin.longitude()) * cos(dest.latitude());
  double b1 = cos(origin.latitude()) * sin(dest.latitude());
  double b2 = sin(origin.latitude()) * cos(dest.latitude()) *
      cos(dest.longitude() - origin.longitude());
  double b = b1 - b2;
  return rad_to_deg(atan2(a, b));
}

double VectorToDegrees(double x, double y) {
  double angle;

  if (x >= 0 && y >= 0) {
    angle = tan(x / y) * 180 / M_PI;
  } else if (x < 0 && y > 0) {
    angle = 90 - tan(x / y) * 180 / M_PI;
  } else if (x < 0 && y < 0) {
    angle = 180 - tan(x / y) * 180 / M_PI;
  } else {
    angle = 90 + tan(x / y) * 180 / M_PI;
  }

  return angle;
}

double BoundToPI(double angle) {
  if (angle < 0) {
    angle = fmod(angle, -M_2_PI);
  } else {
    angle = fmod(angle, M_2_PI);
  }

  if (angle <= -M_PI) {
    angle = angle + M_2_PI;
  } else if (angle > M_PI) {
    angle = angle - M_2_PI;
  }
  return angle;
}

double BoundTo180(double angle) {
  if (angle < 0) {
    angle = fmod(angle, -360);
  } else {
    angle = fmod(angle, 360);
  }

  if (angle <= -180) {
    angle = angle + 360;
  } else if (angle > 180) {
    angle = angle - 360;
  }

  return angle;
}

int16_t BoundTo180Exact(int16_t angle) {
  if (angle < 0) {
    angle = static_cast<int16_t> (angle % -360);
  } else {
    angle = static_cast<int16_t> (angle % 360);
  }

  if (angle <= -180) {
    angle = static_cast<int16_t> (angle + 360);
  } else if (angle > 180) {
    angle = static_cast<int16_t> (angle - 360);
  }

  return angle;
}

int32_t BoundTo180Exact(int32_t angle, int32_t multiplier) {
  if (angle < 0) {
    angle = angle % (-360 * multiplier);
  } else {
    angle = angle % (360 * multiplier);
  }

  if (angle <= -180 * multiplier) {
    angle = angle + 360 * multiplier;
  } else if (angle > 180 * multiplier) {
    angle = angle - 360 * multiplier;
  }

  return angle;
}

bool IsAngleBetween(double first_angle, double middle_angle, double second_angle) {
  first_angle = BoundTo180(first_angle);
  middle_angle = BoundTo180(middle_angle);
  second_angle = BoundTo180(second_angle);
  double delta1 = calculate_angle_delta(first_angle, middle_angle);
  double delta2 = calculate_angle_delta(middle_angle, second_angle);

  if (fabs(delta1 + delta2) >= 180) {
    return false;
  } else {
    return (delta1 > 0 && delta2 > 0) || (delta1 < 0 && delta2 < 0);
  }
}

double calculate_angle_delta(double first_angle, double second_angle) {
  double difference = second_angle - first_angle;
  if (difference > 180) {
    difference -= 360;
  }

  if (difference < -180) {
    difference += 360;
  }

  return difference;
}

int32_t Long10Pow(int32_t n) {
  int32_t r = 1;
  const int32_t x = 10;

  while (n) {
    if (n & 1) {
      r *= x;
      n--;
    } else {
      r *= x * x;
      n -= 2;
    }
  }

  return r;
}

bool are_equal(double a, double b, double epsilon) {
  return fabs(a - b) < epsilon;
}

bool are_equal(double a, double b) {
  return are_equal(a, b, 0.000001);
}

bool almost_equal(const Eigen::Vector3f &point1, const Eigen::Vector3f &point2, const double tolerance) {
  return ((point1 - point2).norm() < tolerance);
}

double deg_to_rad(double degrees) {
  return (M_PI / 180.) * degrees;
}

double rad_to_deg(double radians) {
  return 180. * (radians / M_PI);
}

void CoordToPoint(const GPSCoordinateFast &coord, Eigen::Vector3f *surface_position, double r) {
  // converting spherical coordinates to cartesian coordinates
  (*surface_position)(0) = static_cast<float>(cos(coord.latitude()) * cos(coord.longitude()) * r);
  (*surface_position)(1) = static_cast<float>(cos(coord.latitude()) * sin(coord.longitude()) * r);
  (*surface_position)(2) = static_cast<float>(sin(coord.latitude()) * r);
}

bool PointToCoord(const Eigen::Vector3f &surface_position, GPSCoordinateFast *coord) {
  bool on_surface;
  // check if point is on sphere (within tolerance)
  if (is_on_sphere(surface_position)) {
    on_surface = true;
    // converting cartesian coordinates to spherical coordinates
    double lat = atan2(surface_position(2), sqrt(pow(surface_position(0), 2) + pow(surface_position(1), 2)));
    double lng = atan2(surface_position(1), surface_position(0));
    (*coord).set_lat_lng(lat, lng);
  } else {
    on_surface = false;
  }
  return on_surface;
}

bool is_on_sphere(const Eigen::Vector3f &point, const double R) {
  // r is distance to origin
  double r = point.norm();
  // check if point is on sphere (within tolerance)
  return (fabs(R - r) < sailbot::kPointTolerance);
}

}  // namespace standard_calc
