#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <eccodes.h>
#include "StandardCalc.h"
#include "fileParse.h"

fileParse::fileParse(char *filename) {
    err = 0;
    in = fopen (filename, "r");
    if (!in) {
        printf ("ERROR: unable to open input file %s\n", filename);
    }

    /* create new handle from a message in a file */
    h = codes_handle_new_from_file (0, in, PRODUCT_GRIB, &err);
    if (h == NULL) {
        printf ("Error: unable to create handle from file %s\n", filename);
    }

    CODES_CHECK (codes_get_long (h, "numberOfPoints", &numberOfPoints), 0);
    CODES_CHECK (codes_set_double (h, "missingValue", missing), 0);

    lats = new double [numberOfPoints];
    lons = new double [numberOfPoints];
    values = new double[numberOfPoints];

    CODES_CHECK (codes_grib_get_data (h, lats, lons, values), 0);

    for (int i = 0; i < numberOfPoints; ++i) {
        lats[i] = StandardCalc::BoundTo180(lats[i]);
        lons[i] = StandardCalc::BoundTo180(lons[i]);
    }

    codes_handle_delete (h);
    fclose (in);
}