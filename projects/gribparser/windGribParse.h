//
// Created by andrea on 02/03/19.
//

#ifndef CXX_WINDGRIBPARSE_H
#define CXX_WINDGRIBPARSE_H
#include <eccodes.h>
#include <string>
#include <iostream>
#include <iostream>
#include <vector>
#include "../src/logic/StandardCalc.h"

#endif //CXX_WINDGRIBPARSE_H

#define PI 3.14159265

class WindFileParse {
public:
    long number_of_points_;
    const double kMissing = 9999.0;
    std::vector<double> lats;
    std::vector<double> lons;
    std::vector<double> v_values;
    std::vector<double> u_values;
    std::vector<double> angles;
    std::vector<double> magnitudes;

    WindFileParse(const std::string filename);
    double calcMagnitude(const double u_comp,const double v_comp);
    double calcAngle(const double u_comp,const double v_comp);

private:
    int err;
    FILE *in;
    codes_handle *lib_handle;
    int code_handle_iteration;

};