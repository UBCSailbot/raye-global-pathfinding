// Copyright 2017 UBC Sailbot
#ifndef GRIB_GRIBPARSE_H_
#define GRIB_GRIBPARSE_H_

#include <eccodes.h>
#include <string>
#include <iostream>
#include <vector>
#include "logic/StandardCalc.h"

#define PI 3.14159265

class gribParse {
 public:
        explicit gribParse(const std::string & filename, int time_steps = 4, bool use_csvs = false);
        static double calcMagnitude(const double u_comp, const double v_comp);
        static double calcAngle(const double u_comp, const double v_comp);
        void saveKML(bool preserveKml);

        int64_t number_of_points_;
        std::vector<double> lats;
        std::vector<double> lons;
        std::vector<double> wind_gust;
        std::vector<double> cloudcover;
        std::vector<double> precipitation;
        std::vector<double> pressure;
        std::vector<double> cape;
        std::vector<double> temperature;
        std::vector<std::vector<double>> angles;
        std::vector<std::vector<double>> magnitudes;
        std::vector<double> hexes;
        std::vector<std::vector<double>> missing;

 private:
        void saveToCsv2D(const std::vector<std::vector<double>> & array2D, const std::string & csvfilename);
        std::vector<std::vector<double>> convert1Dto2D(const std::vector<double> & array1D, int numRows, int numCols);
        std::vector<double> convert2Dto1D(const std::vector<std::vector<double>> & array2D);
        std::vector<std::vector<double>> readCsv(const std::string & csvfilename);
        std::vector<std::vector<double>> reverseColumns(const std::vector<std::vector<double>> & array2D);

        int err;
        FILE *in;
        codes_handle *lib_handle;
        const double kMissing = 9999.0;
};

#endif  // GRIB_GRIBPARSE_H_
