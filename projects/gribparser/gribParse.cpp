#include "gribParse.h"

fileParse::fileParse(std::string filename) {
    err = 0;
    in = fopen (filename.c_str(), "r");
    if (!in) {
        cout << "ERROR: unable to open input file" << filename << endl;
    }

    /* create new handle from a message in a file */
    h = codes_handle_new_from_file (nullptr, in, PRODUCT_GRIB, &err);
    if (h == nullptr) {
        cout << "ERROR: unable to create handle from file" << filename << endl;
    }

    CODES_CHECK (codes_get_long (h, "numberOfPoints", &numberOfPoints), 0);
    CODES_CHECK (codes_set_double (h, "missingValue", missing), 0);

    lats = new double [numberOfPoints];
    lons = new double [numberOfPoints];
    values = new double[numberOfPoints];

    CODES_CHECK (codes_grib_get_data (h, lats, lons, values), 0);

    for (int i = 0; i < numberOfPoints; ++i) {
        lats[i] = standard_calc::BoundTo180(lats[i]);
        lons[i] = standard_calc::BoundTo180(lons[i]);
    }

    codes_handle_delete (h);
    fclose (in);
}