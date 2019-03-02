// Copyright 2017 UBC Sailbot


#include <vector>
#include "windGribParse.h"
using namespace std;


double WindFileParse::calcAngle(double u_comp, double v_comp) {
    return 270-(atan(v_comp/u_comp) * 180 / PI);
}

double WindFileParse::calcMagnitude(double u_comp, double v_comp) {
    return (sqrt(pow(u_comp,2) + pow(v_comp,2)))/0.514444;
}

WindFileParse::WindFileParse(const std::string filename) {
    err = 0;
    in = fopen(filename.c_str(), "r");
    if (!in) {
        cout << "ERROR: unable to open input file" << filename << endl;
    }

    code_handle_iteration = 1;
    while (((lib_handle = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err)) != NULL) ){
        if((code_handle_iteration == 5 || code_handle_iteration ==6)) {
            CODES_CHECK(err, 0);

            CODES_CHECK (codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
            CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

            lats.resize(number_of_points_);
            lons.resize(number_of_points_);
            v_values.resize(number_of_points_);
            u_values.resize(number_of_points_);

            if(code_handle_iteration == 5) {
                u_values.resize(number_of_points_);
                CODES_CHECK (codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values.data()), 0);
            } else {
                v_values.resize(number_of_points_);
                CODES_CHECK (codes_grib_get_data(lib_handle.data(), lats, lons.data(), v_values.data()), 0);
            }
            codes_handle_delete(lib_handle);
        }
        code_handle_iteration++;
    }

    //generate new array for angles and magnitudes
    angles.resize(number_of_points_);
    magnitudes.resize(number_of_points_);

    for (int i = 0; i < number_of_points_; ++i) {
        angles[i] = calcAngle(u_values[i],v_values[i]);
        magnitudes[i] = calcMagnitude(u_values[i],v_values[i]);
        lats[i] = standard_calc::BoundTo180(lats[i]);
        lons[i] = standard_calc::BoundTo180(lons[i]);
    }

//    for (int i = 0; i < number_of_points_; ++i) {
//        std::cout << "Lat: " << lats[i] << std::setw(10) << "\tLong: " << lons[i]  << std::setw(10)
//                  << "\tU-comp: " << u_values[i]  << std::setw(10) << "\tV-comp: " << v_values[i] << std::setw(10)
//                  << "\tangles: " << angles[i]  << std::setw(20) << "\tmagnitudes: " << magnitudes[i] << std::endl ;
//    }

    fclose(in);
    return 0;
};

