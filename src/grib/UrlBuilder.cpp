// Copyright 2017 UBC Sailbot
#include "UrlBuilder.h"
#include <iostream>
#include <vector>

/**
 * Builds the URL based on the 5 input coordinates given
 * (North, South, East, West, Weather Type).
 * Weather Type is enumerated based on the following:
 * 0 - CAPE (stormy weather)
 * 1 - Wind
 * @param argv 5 input coordinates (North, South, East, West, Weather Type)
 * @return constructed URL for downloading GRIB file
 */
std::string UrlBuilder::BuildURL(const std::string & north, const std::string & south,
                                 const std::string & east, const std::string & west) {
  const std::string fixed_east = std::to_string(std::stoi(east) > 0 ? std::stoi(east) : std::stoi(east)+360);
  const std::string fixed_west = std::to_string(std::stoi(west) > 0 ? std::stoi(west) : std::stoi(west)+360);

  static const std::string
      kBaseGlobalUrlString = "http://forecast.predictwind.com/grib/custom?I=3&Z=100&L=1&M=g&compress=false";
  static const std::string kUsername = "captain%40ubcsailbot.org";
  static const std::string kPassword = "QeUn6fUywUG5";

  // construct URL
  std::string url(kBaseGlobalUrlString);
  url.append("&N=" + north + "&S=" + south + "&E=" + fixed_east + "&W=" + fixed_west);
  url.append("&V=WRCTPaG");
  url.append("&username=" + kUsername + "&password=" + kPassword);

  std::cout << url << std::endl;
  return url;
}
