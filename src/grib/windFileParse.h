// Copyright 2019 UBC Sailbot
#ifndef GRIB_WINDFILEPARSE_H_
#define GRIB_WINDFILEPARSE_H_
#include <eccodes.h>
#include <string>
#include <iostream>
#include <vector>
#include "logic/StandardCalc.h"


#define PI 3.14159265

class WindFileParse {
 public:
    int64_t number_of_points_;
    std::vector<double> lats;
    std::vector<double> lons;
    std::vector<double> angles;
    std::vector<double> magnitudes;
    std::vector<bool> missing;

    explicit WindFileParse(const std::string filename);
    static double calcMagnitude(const double u_comp, const double v_comp);
    static double calcAngle(const double u_comp, const double v_comp);

 private:
    int err;
    FILE *in;
    codes_handle *lib_handle;
    int code_handle_iteration;
    std::vector<double> v_values;
    std::vector<double> u_values;
    const double kMissing = 9999.0;
};


#endif  // GRIB_WINDFILEPARSE_H_
