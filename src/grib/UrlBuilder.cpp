// Copyright 2017 UBC Sailbot
#include "UrlBuilder.h"
#include <iostream>
#include <vector>

/**
 * Builds the URL based on the 4 input coordinates given
 * @param argc Number of Arguments
 * @param argv 4 input coordinates (North, South, East, West)
 * @return
 */
std::string UrlBuilder::BuildURL(std::string north, std::string south, std::string east, std::string west, int weather_type) {

  static const std::string
      kBaseGlobalUrlString = "http://forecast.predictwind.com/grib/custom?I=3&Z=100&L=1&M=g&compress=false";
  static const std::string kUsername = "captain%40ubcsailbot.org";
  static const std::string kPassword = "QeUn6fUywUG5";

  std::string weather_type_const;

  //CAPE - a
  //wind - w
  if (weather_type == 0)
    weather_type_const = "a";
  else if (weather_type == 1)
    weather_type_const = "w";

  std::string url(kBaseGlobalUrlString);
  url.append("&N=" + north + "&S=" + south + "&E=" + east + "&W=" + west);
  url.append("&V=" + weather_type_const);
  url.append("&username=" + kUsername + "&password=" +kPassword);


  std::cout << url << std::endl;
  return url;
}

