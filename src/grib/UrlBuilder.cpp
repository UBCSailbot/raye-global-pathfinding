#include "UrlBuilder.h"
#include <iostream>
#include <vector>

/**
 * Builds the URL based on the 4 input coordinates given
 * @param argc Number of Arguments
 * @param argv 4 input coordinates (North, South, East, West)
 * @return
 */
std::string UrlBuilder::BuildURL(std::string north, std::string south, std::string east, std::string west) {
  static const std::string
      kBaseGlobalUrlString = "http://forecast.predictwind.com/grib/custom?I=3&Z=100&V=a&L=1&M=g&compress=false";
  static const std::string kUsername = "captain%40ubcsailbot.org";
  static const std::string kPassword = "QeUn6fUywUG5";

  std::string url(kBaseGlobalUrlString);
  url.append("&N=" + north + "&S=" + south + "&E=" + east + "&W=" + west + "&username=" + kUsername + "&password=" +kPassword);

  return url;
}

