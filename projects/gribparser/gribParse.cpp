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

    CODES_CHECK(codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
    CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

    lats_ = new double[number_of_points_];
    lons_ = new double[number_of_points_];
    values_ = new double[number_of_points_];

    CODES_CHECK(codes_grib_get_data(lib_handle, lats_, lons_, values_), 0);

    for (int i = 0; i < number_of_points_; ++i) {
        lats_[i] = standard_calc::BoundTo180(lats_[i]);
        lons_[i] = standard_calc::BoundTo180(lons_[i]);
    }

    codes_handle_delete(lib_handle);
    fclose(in);
}