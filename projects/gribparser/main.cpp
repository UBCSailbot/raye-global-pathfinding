#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include "eccodes.h"
#include "gribParse.h"

using namespace std;

/**
 * Downloads and parses a GRIB data file based on the 4 input coordinates (North South East West)
 * @param argc number of arguments
 * @param argv 4 input coordinates (North, South, East, West)
 * @return
 */

int main(int argc, char *argv[]) {
  std::string file_name = "data.grb";

  if (argc == 5) {
    std::string north = argv[1];
    std::string south = argv[2];
    std::string east = argv[3];
    std::string west = argv[4];
    string url = UrlBuilder::BuildURL(north, south, east, west);
    UrlDownloader::Downloader(&url[0]);
  } else {
    cout << "Function requires 4 arguments" << endl;
    return 0;
  }

  FileParse file = FileParse(file_name);

  // print out result from parsing file

  for (int i = 0; i < file.number_of_points_; ++i) {
    if (file.vals[i] != file.kMissing) {
      cout << "Lat: " << file.lats[i] << "\t Long: " << file.lons[i] << "\tVal: " << file.vals[i] << endl;
    }
  }

  return 0;
}