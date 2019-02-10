#include "UrlBuilder.h"
#include <iostream>

/**
 * Builds the URL based on the 4 input coordinates given
 * @param argc Number of Arguments
 * @param argv 4 input coordinates (North, South, East, West)
 * @return
 */
std::string UrlBuilder::BuildURL(const int argc, char **argv) {
  static const std::string
      kBaseGlobalUrlString = "http://forecast.predictwind.com/grib/custom?I=3&Z=100&V=a&L=1&M=g&compress=false";
  static const std::string kUsername = "captain%40ubcsailbot.org";
  static const std::string kPassword = "QeUn6fUywUG5";

  std::string url(kBaseGlobalUrlString);
  std::string prefix[5] = {"", "&N=", "&S=", "&E=", "&W="};
  for (int i = 1; i < argc; i++) {
    url.append(prefix[i]);
    url.append(argv[i]);
  }

  url.append("&username=");
  url.append(kUsername);
  url.append("&password=");
  url.append(kPassword);

  return url;
}