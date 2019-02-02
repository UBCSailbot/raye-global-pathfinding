//
// Created by Choy Ming Xuan on 2018-11-16.
//

#ifndef GRIB_DOWNLOADER_URLBUILDER_H
#define GRIB_DOWNLOADER_URLBUILDER_H

#include <string>
using namespace std;

class UrlBuilder {
public:
    static std::string BuildURL(const int argc , char **argv);

};

#endif //GRIB_DOWNLOADER_URLBUILDER_H