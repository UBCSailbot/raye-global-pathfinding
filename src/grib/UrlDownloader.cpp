// Copyright 2017 UBC Sailbot
#include "UrlDownloader.h"
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>

// Downloads the file based off the url given
int UrlDownloader::Downloader(const std::string url) {
  CURL *curl;
  FILE *fp;
  CURLcode res;

  char out_file_name[FILENAME_MAX] = "data.grb";
  curl = curl_easy_init();
  if (curl) {
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
  return 0;
}

