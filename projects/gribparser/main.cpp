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
    char filename[] = "data.grb";
    const double missing = 9999.0;
    if (argc == 5) {
        string url = urlBuilder::buildURL(argc, argv);
        urlDownloader::downloader(&url[0]);
    }
    else {
        cout << "Function requires 4 arguments" << endl;
        return 0;
    }

    fileParse file = fileParse(filename);

    long numberOfPoints = file.numberOfPoints;

    for (int i = 0; i < numberOfPoints; ++i) {
        if (file.values[i] != missing) {
            cout << "Lat: " << file.lats[i] << "\t Long: " << file.lons[i] << "\tVal: " << file.values[i] << endl;
        }
    }

    return 0;
}