// Copyright 2016 UBC Sailbot

#include "logic/StandardCalc.h"

#include <cmath>

#include <common/GeneralDefs.h>

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

double AngleBetweenTwoCoords(const GPSCoordinateFast &origin, const GPSCoordinateFast &dest) {
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

double FindCosLawAngle(double a, double b, double c) {
  if ((a < 1) || (b < 1) || (c < 1)) {
    return 0;
  } else {
    return acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
  }
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
  double difference = first_angle - second_angle;

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

double deg_to_rad(double degrees) {
  return (M_PI / 180.) * degrees;
}
double rad_to_deg(double radians) {
  return 180. * (radians / M_PI);
}

double low_pass_filter_update(double current, double raw, double beta) {
  return current - beta * (current - raw);
}

}  // namespace standard_calc
