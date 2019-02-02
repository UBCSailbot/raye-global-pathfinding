//
// Created by andrea on 12/01/19.
//

#ifndef GRIB_PARSER_FILE_PARSE_H
#define GRIB_PARSER_FILE_PARSE_H

#include <eccodes.h>
#include <string>
#include <iostream>
#include <iostream>
#include "../src/logic/StandardCalc.h"
using namespace std;

class fileParse {
public:
    fileParse(std::string filename);
    double *lats, *lons, *values;
    long numberOfPoints;

private:
    int err;
    FILE *in;
    codes_handle *h;
    const double missing = 9999.0;


};

#endif //GRIB_PARSER_FILE_PARSE_H
