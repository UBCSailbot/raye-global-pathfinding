#include "urlBuilder.h"
#include <iostream>

string urlBuilder::buildURL(int argc, char **argv) {
    static const string BASE_GLOBAL_URL_STRING = "http://forecast.predictwind.com/grib/custom?I=3&Z=100&V=a&L=1&M=g&compress=false";
    static const string USERNAME = "captain%40ubcsailbot.org";
    static const string PASSWORD = "QeUn6fUywUG5";

    string url(BASE_GLOBAL_URL_STRING);
    string prefix[5] = {"","&N=","&S=","&E=","&W="};
    for(int i = 1; i < argc; i++){
        url.append(prefix[i]);
        url.append(argv[i]);
    }

    url.append("&username=");
    url.append(USERNAME);
    url.append("&password=");
    url.append(PASSWORD);

    return url;
}