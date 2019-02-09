#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "eccodes.h"
#include "urlDownloader.h"
#include "urlBuilder.h"
#include "gribParse.h"

using namespace std;

/**
 * Downloads and parses a GRIB data file based on the 4 input coordinates (North South East West)
 * @param argc number of arguments
 * @param argv 4 input coordinates (North, South, East, West)
 * @return
 */

int main (int argc, char **argv)
{
    std::string file_name = "data.grb";

    if (argc == 5) {
        string url = UrlBuilder::BuildURL(argc, argv);
        UrlDownloader::Downloader(&url[0]);
    }
    else {
        cout << "Function requires 4 arguments" << endl;
        return 0;
    }

    FileParse file = FileParse(file_name);

    // print out result from parsing file

    for (int i = 0; i < file.number_of_points_; ++i) {
        if (file.values_[i] != file.kMissing) {
            cout << "Lat: " << file.lats_[i] << "\t Long: " << file.lons_[i] << "\tVal: " << file.values_[i] << endl;
        }
    }

    return 0;
}