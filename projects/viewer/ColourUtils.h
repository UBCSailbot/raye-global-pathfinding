// Copyright 2017 UBC Sailbot

#ifndef COLOURUTILS_H_
#define COLOURUTILS_H_

#include <cmath>

// Originally from:
// http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

struct RGB {
  /// A fraction between 0 and 1
  float r;
  /// A fraction between 0 and 1
  float g;
  /// A fraction between 0 and 1
  float b;
};

struct HSV {
  /// Angle in degrees (0-360)
  float h;
  /// A fraction between 0 and 1
  float s;
  /// A fraction between 0 and 1
  float v;
};

/**
 * @param in RBG Colour
 * @return HSV equivalent of |in|
 */
inline static HSV rgb_to_hsv(RGB in) {
  HSV out;
  float min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min < in.b ? min : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max > in.b ? max : in.b;

  // v
  out.v = max;
  delta = max - min;
  if (delta < 0.00001f) {
    out.s = 0;
    // Undefined, maybe nan?
    out.h = 0;
    return out;
  }
  if (max > 0.0f) {
    // NOTE: if Max is == 0, this divide would cause a crash
    // s
    out.s = (delta / max);
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, v is undefined
    out.s = 0.0;
    // its now undefined
    out.h = NAN;
    return out;
  }
  // > accounts for floating point errors
  if (in.r >= max) {
    // Between yellow & magenta
    out.h = (in.g - in.b) / delta;
  } else if (in.g >= max) {
    // Between cyan & yellow
    out.h = 2.0f + (in.b - in.r) / delta;
  } else {
    // Between magenta & cyan
    out.h = 4.0f + (in.r - in.g) / delta;
  }

  // Degrees
  out.h *= 60.0;

  if (out.h < 0.0)
    out.h += 360.0;

  return out;
}

/**
 * @param in HSV Colour
 * @return RBG equivalent of |in|
 */
inline RGB hsv_to_rgb(HSV in) {
  float hh, p, q, t, ff;
  int64_t i;
  RGB out;

  // < accounts for floating point errors
  if (in.s <= 0.0) {
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0) {
    hh = 0.0;
  }

  hh /= 60.0;
  i = static_cast<int64_t> (hh);
  ff = hh - i;
  p = in.v * (1.0f - in.s);
  q = in.v * (1.0f - (in.s * ff));
  t = in.v * (1.0f - (in.s * (1.0f - ff)));

  switch (i) {
    case 0:out.r = in.v;
      out.g = t;
      out.b = p;
      break;
    case 1:out.r = q;
      out.g = in.v;
      out.b = p;
      break;
    case 2:out.r = p;
      out.g = in.v;
      out.b = t;
      break;

    case 3:out.r = p;
      out.g = q;
      out.b = in.v;
      break;
    case 4:out.r = t;
      out.g = p;
      out.b = in.v;
      break;
    case 5:
    default:out.r = in.v;
      out.g = p;
      out.b = q;
      break;
  }
  return out;
}

#endif  // COLOURUTILS_H_
