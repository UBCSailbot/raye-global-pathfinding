// Copyright 2017 UBC Sailbot
#include "gribParse.h"
#include <vector>

/**
 * Translates GRIB file into array of lattitudes, longitudes, and corresponding values
 * @param filename of target GRIB file
 * @return
 */

gribParse::gribParse(const std::string filename) {
  err = 0;
  in = fopen(filename.c_str(), "r");
  if (!in) {
    std::cout << "ERROR: unable to open input file" << filename << std::endl;
  }

  std::vector<double> u_values;
  std::vector<double> v_values;

  for (int code_handle_iteration = 1; ((lib_handle = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err)) != NULL) && code_handle_iteration <= 8; code_handle_iteration++) {
      CODES_CHECK(err, 0);
      CODES_CHECK (codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
      CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);
      lats.resize(number_of_points_);
      lons.resize(number_of_points_);

      switch(code_handle_iteration) {
          case 1:
              cape.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), cape.data()), 0);
              break;
          case 2:
              wind_gust.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), wind_gust.data()), 0);
              break;
          case 3:
              precipitation.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), precipitation.data()), 0);
              break;
          case 4:
              pressure.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), pressure.data()), 0);
              break;
          case 5:
              cloudcover.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), cloudcover.data()), 0);
              break;
          case 6:
              temperature.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), temperature.data()), 0);
              break;
          case 7:
              u_values.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), u_values.data()), 0);
              break;
          case 8:
              v_values.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), v_values.data()), 0);
              break;
          default:
              break;

      }
      codes_handle_delete(lib_handle);
  }

    angles.resize(number_of_points_);
    magnitudes.resize(number_of_points_);
    missing.resize(number_of_points_);

    CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

    //adjust latitude and longitude, and generate resultant angles and magnitudes
    for (int i = 0; i < number_of_points_; ++i) {
        if(u_values[i] == kMissing)
            missing[i] = true;
        else
            missing[i] = false;

        angles[i] = calcAngle(u_values[i],v_values[i]);
        magnitudes[i] = calcMagnitude(u_values[i],v_values[i]);
        lats[i] = standard_calc::BoundTo180(lats[i]);
        lons[i] = standard_calc::BoundTo180(lons[i]);
    }

  fclose(in);
}

/**
 * Calculates angle, relative to an origin at the 12 o'clock position
 * @param u and v vector component of the wind
 * @return angle in degrees, relative to new position
 */
double gribParse::calcAngle(double u_comp, double v_comp) {
    return 270-(atan(v_comp/u_comp) * 180 / PI);
}

/**
 * Calculates magnitude of wind vector
 * @param u and v vector component of the wind
 * @return magnitude of vector in knots/s
 */
double gribParse::calcMagnitude(double u_comp, double v_comp) {
    return (sqrt(pow(u_comp,2) + pow(v_comp,2)))/0.514444;
}
