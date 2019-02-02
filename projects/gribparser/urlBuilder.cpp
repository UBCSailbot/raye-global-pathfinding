#include "urlBuilder.h"
#include <iostream>

string UrlBuilder::BuildURL(const int argc, char **argv) {
    static const string kBaseGlobalUrlString= "http://forecast.predictwind.com/grib/custom?I=3&Z=100&V=a&L=1&M=g&compress=false";
    static const string kUsername = "captain%40ubcsailbot.org";
    static const string kPassword = "QeUn6fUywUG5";

    string url(kBaseGlobalUrlString);
    string prefix[5] = {"","&N=","&S=","&E=","&W="};
    for (int i = 1; i < argc; i++){
        url.append(prefix[i]);
        url.append(argv[i]);
    }

    url.append("&username=");
    url.append(kUsername);
    url.append("&password=");
    url.append(kPassword);

    return url;
}