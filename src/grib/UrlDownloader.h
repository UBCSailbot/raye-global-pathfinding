// Copyright 2017 UBC Sailbot
#ifndef GRIB_URLDOWNLOADER_H_
#define GRIB_URLDOWNLOADER_H_

#include <string>

class UrlDownloader {
 public:
  static int Downloader(const std::string url);
};

#endif  // GRIB_URLDOWNLOADER_H_
