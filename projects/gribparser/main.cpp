#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include "eccodes.h"
#include "grib/gribParse.h"
#include "grib/windFileParse.h"
#include <iomanip>

using namespace std;

/**
 * Downloads and parses a GRIB data file based on the 5 input coordinates (North South East West, Weather Type)
 * @param argc number of arguments
 * @param argv 5 input coordinates (North, South, East, West, Weather Type)
 * @return
 */

int main(int argc, char *argv[]) {
    std::string file_name = "data.grb";
    static const int kCAPEData = 0;
    static const int kWindData = 1;

    if (argc == 6) {
        std::string north = argv[1];
        std::string south = argv[2];
        std::string east = argv[3];
        std::string west = argv[4];
        std::string weather_type = argv[5];

        string url = UrlBuilder::BuildURL(north, south, east, west, stoi(weather_type));
        UrlDownloader::Downloader(&url[0]);


        // print out result from parsing file
        if (stoi(weather_type) == kCAPEData) {
            FileParse file = FileParse(file_name);
            for (int i = 0; i < file.number_of_points_; ++i) {
                if (!file.missing[i]) {
                    cout << "Lat: " << file.lats[i] << std::setw(10) << "\t Long: " << file.lons[i] << std::setw(10) << "\tVal: " << file.vals[i] << endl;
                }
            }
        } else if (stoi(weather_type) == kWindData) {
            WindFileParse file = WindFileParse(file_name);
            for (int i = 0; i < file.number_of_points_; ++i) {
                if (!file.missing[i]) {
                    std::cout << "Lat: " << file.lats[i] << std::setw(10) << "\tLong: " << file.lons[i] << std::setw(10)
                              << "\tangle: " << file.angles[i] << std::setw(20) << "\tmagnitude: " << file.magnitudes[i]
                              << std::endl;
                }
            }
        }
  } else {
    cout << "Function requires 5 arguments" << endl;

  }

  return 0;
}