// Copyright 2017 UBC Sailbot
#ifndef GRIB_URLBUILDER_H_
#define GRIB_URLBUILDER_H_

#include <string>

class UrlBuilder {
 public:
    static std::string BuildURL(const std::string & north, const std::string & south,
                                const std::string & east, const std::string & west);
};

#endif  // GRIB_URLBUILDER_H_
