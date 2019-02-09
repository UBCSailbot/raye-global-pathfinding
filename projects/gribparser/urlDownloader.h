#ifndef GRIB_DOWNLOADER_URLDOWNLOADER_H
#define GRIB_DOWNLOADER_URLDOWNLOADER_H

#include <string>

class UrlDownloader {
    public:
        static int Downloader(const std::string url);
};


#endif //GRIB_DOWNLOADER_URLDOWNLOADER_H
