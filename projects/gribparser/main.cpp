#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include "eccodes.h"
#include "grib/gribParse.h"
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

    if (argc == 5) {
        std::string north = argv[1];
        std::string south = argv[2];
        std::string east = argv[3];
        std::string west = argv[4];

        string url = UrlBuilder::BuildURL(north, south, east, west);
        UrlDownloader::Downloader(url);

        gribParse file = gribParse(file_name);
        file.saveKML();
        for (int i = 0; i < file.number_of_points_ && i < 100; ++i) {
            if (!file.missing[i]) {
                cout << "Lat: " << file.lats[i] << std::setw(10) << "\t Long: " << file.lons[i] << std::setw(10) << "\tMag: " << file.magnitudes[i] << "\tDir: " << file.angles[i] << endl;
            }
        }

  } else {
    cout << "Function requires 4 arguments" << endl;

  }

  return 0;
}
