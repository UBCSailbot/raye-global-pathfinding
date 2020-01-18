// Copyright 2017 UBC Sailbot
#ifndef GRIB_PARSER_GRIB_PARSE_H
#define GRIB_PARSER_GRIB_PARSE_H

#include <eccodes.h>
#include <string>
#include <iostream>
#include <vector>
#include "logic/StandardCalc.h"

#define PI 3.14159265

class gribParse {
    public:
        long number_of_points_;
        std::vector<double> lats;
        std::vector<double> lons;
        std::vector<double> wind_gust;
        std::vector<double> cloudcover;
        std::vector<double> precipitation;
        std::vector<double> pressure;
        std::vector<double> cape;
        std::vector<double> temperature;
        std::vector<double> angles;
        std::vector<double> magnitudes;
        std::vector<double> hexes;
        std::vector<bool> missing;

        gribParse(const std::string filename);
        double static calcMagnitude(const double u_comp,const double v_comp);
        double static calcAngle(const double u_comp,const double v_comp);
        void saveKML();

 private:
        int err;
        FILE *in;
        codes_handle *lib_handle;
        const double kMissing = 9999.0;
};

#endif //GRIB_PARSER_GRIB_PARSE_H
