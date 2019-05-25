// Copyright 2017 UBC Sailbot
#ifndef GRIB_URLBUILDER_H_
#define GRIB_URLBUILDER_H_

#include <string>

class UrlBuilder {
 public:
    static std::string BuildURL(std::string north, std::string south, std::string east, std::string west,
        int weather_type);
};

#endif  // GRIB_URLBUILDER_H_
