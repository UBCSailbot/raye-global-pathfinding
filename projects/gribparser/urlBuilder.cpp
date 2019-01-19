#include "urlBuilder.h"
#include <iostream>

#define BASE_GLOBAL_URL_STRING "http://forecast.predictwind.com/grib/custom?I=3&Z=100&V=a&L=1&M=g&compress=false"
#define USERNAME "captain%40ubcsailbot.org"
#define PASSWORD  "QeUn6fUywUG5"

using namespace std;

string urlBuilder::buildURL(int argc, char **argv) {
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






