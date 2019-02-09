#include <vector>
#include "gribParse.h"

FileParse::FileParse(const std::string filename) {
    err = 0;
    in = fopen(filename.c_str(), "r");
    if (!in) {
        cout << "ERROR: unable to open input file" << filename << endl;
    }

    /* create new handle from a message in a file */
    lib_handle = codes_handle_new_from_file(nullptr, in, PRODUCT_GRIB, &err);

    if (lib_handle == nullptr) {
        cout << "ERROR: unable to create handle from file" << filename << endl;
    }

    // Sets the number of points from the GRIB file
    CODES_CHECK(codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
    // Sets the value for which the data is missing from the GRIB file
    CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

    lats.resize(number_of_points_);
    lons.resize(number_of_points_);
    vals.resize(number_of_points_);

    CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), vals.data()), 0);

    for (int i = 0; i < number_of_points_; ++i) {
        lats[i] = standard_calc::BoundTo180(lats[i]);
        lons[i] = standard_calc::BoundTo180(lons[i]);
    }

    codes_handle_delete(lib_handle);
    fclose(in);
}