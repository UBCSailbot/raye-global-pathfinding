#ifndef GRIB_DOWNLOADER_URLBUILDER_H
#define GRIB_DOWNLOADER_URLBUILDER_H

#include <string>

class UrlBuilder {
 public:
  static std::string BuildURL(const int argc, char **argv);

};

#endif //GRIB_DOWNLOADER_URLBUILDER_H