// Copyright 2017 UBC Sailbot


#include <vector>
#include "windGribParse.h"
using namespace std;

/**
 * Calculates angle, relative to an origin at the 12 o'clock position
 * @param u and v vector component of the wind
 * @return angle in degrees, relative to new position
 */
double WindFileParse::calcAngle(double u_comp, double v_comp) {
    return 270-(atan(v_comp/u_comp) * 180 / PI);
}

/**
 * Calculates magnitude of wind vector
 * @param u and v vector component of the wind
 * @return magnitude of vector in knots/s
 */
double WindFileParse::calcMagnitude(double u_comp, double v_comp) {
    return (sqrt(pow(u_comp,2) + pow(v_comp,2)))/0.514444;
}

/**
 * Translates GRIB file into array of lattitudes, longitudes, and
 * their corresponding u and v wind vectors and
 * resultant magnitudes (knots/s) and directions (degrees)
 * @param filename of target GRIB file
 * @return
 */

WindFileParse::WindFileParse(const std::string filename) {
    err = 0;
    in = fopen(filename.c_str(), "r");
    if (!in) {
        cout << "ERROR: unable to open input file" << filename << endl;
    }

    code_handle_iteration = 1;
    while (((lib_handle = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err)) != NULL) ){

        // only take the 3rd group of code handles, which are accurate for wind data
        // (one group of code handles consists of two code handles
        // (u and v components have a code handle each))
        if((code_handle_iteration == 5 || code_handle_iteration ==6)) {
            CODES_CHECK(err, 0);

            CODES_CHECK (codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
            CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

            lats.resize(number_of_points_);
            lons.resize(number_of_points_);
            missing.resize(number_of_points_);
            v_values.resize(number_of_points_);
            u_values.resize(number_of_points_);

            if(code_handle_iteration == 5) {
                u_values.resize(number_of_points_);
                CODES_CHECK (codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values.data()), 0);
            } else {
                v_values.resize(number_of_points_);
                CODES_CHECK (codes_grib_get_data(lib_handle, lats.data(), lons.data(), v_values.data()), 0);
            }
            codes_handle_delete(lib_handle);
        }
        code_handle_iteration++;
    }

    //generate new array for angles and magnitudes
    angles.resize(number_of_points_);
    magnitudes.resize(number_of_points_);

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
};

