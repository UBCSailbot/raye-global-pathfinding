//
// Created by Choy Ming Xuan on 2018-11-16.
//
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include "urlDownloader.h"

int urlDownloader::downloader(std::string url){
    // cout << url << endl;
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char outfilename[FILENAME_MAX] = "data.grb";
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        fclose(fp);
        std::cout << "Downloaded data.grb" << std::endl;

    }
    return 0;
}

