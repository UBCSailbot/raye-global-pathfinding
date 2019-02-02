/*
 * Copyright 2005-2018 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
 * virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

/*
 * C Implementation: grib_get_data
 *
 * Description: how to get lat/lon/values from a GRIB message
 *
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "eccodes.h"
#include "urlDownloader.h"
#include "urlBuilder.h"
#include "gribParse.h"

using namespace std;

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