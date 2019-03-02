// Copyright 2017 UBC Sailbot
#ifndef GRIB_PARSER_FILE_PARSE_H
#define GRIB_PARSER_FILE_PARSE_H

#include <eccodes.h>
#include <string>
#include <iostream>
#include <iostream>
#include <vector>
#include "../src/logic/StandardCalc.h"

class FileParse {
    public:
        long number_of_points_;
        const double kMissing = 9999.0;
        std::vector<double> lats;
        std::vector<double> lons;
        std::vector<double> vals;

        FileParse(const std::string filename);

    private:
        int err;
        FILE *in;
        codes_handle *lib_handle;

};

#endif //GRIB_PARSER_FILE_PARSE_H