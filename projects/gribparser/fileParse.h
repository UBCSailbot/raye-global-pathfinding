//
// Created by andrea on 12/01/19.
//

#ifndef GRIB_PARSER_FILE_PARSE_H
#define GRIB_PARSER_FILE_PARSE_H

#include <eccodes.h>

class fileParse {
public:
    fileParse(char * filename);
    double *lats, *lons, *values;
    long numberOfPoints;

private:
    int err;
    FILE *in;
    codes_handle *h;
    const double missing = 9999.0;


};

#endif //GRIB_PARSER_FILE_PARSE_H
