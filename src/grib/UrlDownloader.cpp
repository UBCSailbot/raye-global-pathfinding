// Copyright 2017 UBC Sailbot
#include "UrlDownloader.h"
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>

/**
 * Downloads a file based off the URL given
 * @param download URL
 * @return
 */

void UrlDownloader::Downloader(const std::string & url) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    char out_file_name[FILENAME_MAX] = "data.grb";
    FILE *fp;
    fp = fopen(out_file_name, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    fclose(fp);
    std::cout << "Downloaded data.grb" << std::endl;
  }
}

