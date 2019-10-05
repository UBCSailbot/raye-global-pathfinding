// Copyright 2017 UBC Sailbot
#ifndef GRIB_GRIBPARSE_H_
#define GRIB_GRIBPARSE_H_

#include <eccodes.h>
#include <string>
#include <iostream>
#include <vector>
#include "logic/StandardCalc.h"

class FileParse {
 public:
        int64_t number_of_points_;
        std::vector<double> lats;
        std::vector<double> lons;
        std::vector<double> vals;
        std::vector<bool> missing;

        explicit FileParse(const std::string filename);

 private:
        int err;
        FILE *in;
        codes_handle *lib_handle;
        const double kMissing = 9999.0;
};

#endif  // GRIB_GRIBPARSE_H_
