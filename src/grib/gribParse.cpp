// Copyright 2017 UBC Sailbot
#include "gribParse.h"
#include <vector>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include <iomanip>
#include <fstream>
#include <string>

/**
 * Translates GRIB file into array of lattitudes, longitudes, and corresponding values
 * @param filename of target GRIB file
 * @return
 */

gribParse::gribParse(const std::string & filename, int time_steps) {
  if (filename == "csv") {
    // Read saved csv files to get weather information
    // Need to reverse columns because lats ordering issue described below
    // Refer to issue https://github.com/UBCSailbot/global-pathfinding/pull/40 for detailed example
    std::string input_csvs_directory = "input_csvs/";
    std::cout << "Reading in csvs from the following directory: " << input_csvs_directory << std::endl;
    std::cout << "Will segfault if csvs are missing" << std::endl;

    // lats and lons should have shape (number_of_points_)
    lats = convert2Dto1D(reverseColumns(readCsv(input_csvs_directory + "lats2d.csv")));
    lons = convert2Dto1D(reverseColumns(readCsv(input_csvs_directory + "lons2d.csv")));
    number_of_points_ = lats.size();

    // angles and magnitudes should have shape (time_steps, number_of_points_)
    angles.resize(time_steps);
    magnitudes.resize(time_steps);
    for (int i = 0; i < time_steps; i++) {
      std::string angle_filename = input_csvs_directory + "angles2d-" + std::to_string(i) + ".csv";
      std::vector<double> angles_at_time = convert2Dto1D(reverseColumns(readCsv(angle_filename)));
      angles[i] = angles_at_time;
    }
    for (int i = 0; i < time_steps; i++) {
      std::string magnitude_filename = input_csvs_directory + "magnitudes2d-" + std::to_string(i) + ".csv";
      std::vector<double> magnitudes_at_time = convert2Dto1D(reverseColumns(readCsv(magnitude_filename)));
      magnitudes[i] = magnitudes_at_time;
    }
  } else {
    // Open stored grb file
    in = fopen(filename.c_str(), "r");
    if (!in) {
      std::cout << "ERROR: unable to open input file" << filename << std::endl;
    }

    // Parse grb file
    std::vector<std::vector<double>> u_values;
    std::vector<std::vector<double>> v_values;
    err = 0;
    for (int code_handle_iteration = 1;
          ((lib_handle = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err)) != NULL) && code_handle_iteration <= 31;
          code_handle_iteration++) {
        CODES_CHECK(err, 0);
        CODES_CHECK(codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
        CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);
        lats.resize(number_of_points_);
        lons.resize(number_of_points_);


        switch (code_handle_iteration) {
            case 1:
                cape.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), cape.data()), 0);
                break;
            case 2:
                wind_gust.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), wind_gust.data()), 0);
                break;
            case 3:
                precipitation.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), precipitation.data()), 0);
                break;
            case 4:
                pressure.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), pressure.data()), 0);
                break;
            case 5:
                cloudcover.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), cloudcover.data()), 0);
                break;
            case 6:
                temperature.resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), temperature.data()), 0);
                break;
            case 7:
                u_values.resize(time_steps);
                u_values[0].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values[0].data()), 0);
                break;
            case 8:
                v_values.resize(time_steps);
                v_values[0].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), v_values[0].data()), 0);
                break;
            case 15:
                u_values.resize(time_steps);
                u_values[1].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values[1].data()), 0);
                break;
            case 16:
                v_values.resize(time_steps);
                v_values[1].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), v_values[1].data()), 0);
                break;
            case 23:
                u_values.resize(time_steps);
                u_values[2].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values[2].data()), 0);
                break;
            case 24:
                v_values.resize(time_steps);
                v_values[2].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), v_values[2].data()), 0);
                break;
            case 31:
                u_values.resize(time_steps);
                u_values[3].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), u_values[3].data()), 0);
                break;
            case 32:
                v_values.resize(time_steps);
                v_values[3].resize(number_of_points_);
                CODES_CHECK(codes_grib_get_data(lib_handle, lats.data(), lons.data(), v_values[3].data()), 0);
                break;
            default:
                break;
        }
        codes_handle_delete(lib_handle);
    }

    // Setup storing vectors
    angles.resize(time_steps);
    magnitudes.resize(time_steps);
    missing.resize(time_steps);
    u_values.resize(time_steps);
    v_values.resize(time_steps);

    CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

    // adjust latitude and longitude, and generate resultant angles and magnitudes
    for (int i = 0; i < time_steps; i++) {
      angles[i].resize(number_of_points_);
      magnitudes[i].resize(number_of_points_);
      missing[i].resize(number_of_points_);
      u_values[i].resize(number_of_points_);
      v_values[i].resize(number_of_points_);
      for (int j = 0; j < number_of_points_; j++) {
        if (u_values[i][j] == kMissing)
          missing[i][j] = true;
        else
          missing[i][j] = false;

        angles[i][j] = calcAngle(u_values[i][j], v_values[i][j]);
        magnitudes[i][j] = calcMagnitude(u_values[i][j], v_values[i][j]);
        lats[j] = standard_calc::BoundTo180(lats[j]);
        lons[j] = standard_calc::BoundTo180(lons[j]);
      }
    }

    // Calculate number of rows and columns to convert from 1D array to 2D array for csv
    double minLat = *std::min_element(lats.begin(), lats.end());
    double minLon = *std::min_element(lons.begin(), lons.end());
    double maxLat = *std::max_element(lats.begin(), lats.end());
    double maxLon = *std::max_element(lons.begin(), lons.end());
    int numRows = round(maxLat - minLat) + 1;
    int numCols = round(maxLon - minLon) + 1;

    // Write to csv files
    std::string output_csvs_directory = "output_csvs/";
    std::cout << "Writing csvs to directory: " << output_csvs_directory << std::endl;

    // Must reverse columns for saving because b/c lats go in order 21, 22, ..., but want it 48, 47, ...
    // so csv shape matches real shape
    // Lats are like a y position, so larger numbers start at top
    // Don't need for lon b/c like a x position, so smaller numbers start at left, but do for consistency
    std::vector<std::vector<double>> lats2d = reverseColumns(convert1Dto2D(lats, numRows, numCols));
    std::vector<std::vector<double>> lons2d = reverseColumns(convert1Dto2D(lons, numRows, numCols));
    saveToCsv2D(lats2d, output_csvs_directory + "lats2d.csv");
    saveToCsv2D(lons2d, output_csvs_directory + "lons2d.csv");
    for (size_t i = 0; i < magnitudes.size(); i++) {
      std::vector<std::vector<double>> magnitudes2d = reverseColumns(convert1Dto2D(magnitudes.at(i), numRows, numCols));
      saveToCsv2D(magnitudes2d, output_csvs_directory + "magnitudes2d-" + std::to_string(i) + ".csv");
    }
    for (size_t i = 0; i < angles.size(); i++) {
      std::vector<std::vector<double>> angles2d = reverseColumns(convert1Dto2D(angles.at(i), numRows, numCols));
      saveToCsv2D(angles2d, output_csvs_directory + "angles2d-" + std::to_string(i) + ".csv");
    }

    fclose(in);
  }
}

/**
 * Calculates angle, relative to an origin at the 12 o'clock position
 * @param u and v vector component of the wind
 * @return angle in degrees, relative to new position
 */
double gribParse::calcAngle(double u_comp, double v_comp) {
    return 270-(atan(v_comp/u_comp) * 180 / PI);
}

/**
 * Calculates magnitude of wind vector
 * @param u and v vector component of the wind
 * @return magnitude of vector in knots/s
 */
double gribParse::calcMagnitude(double u_comp, double v_comp) {
    return (sqrt(pow(u_comp, 2) + pow(v_comp, 2)))/0.514444;
}

void gribParse::saveKML() {
    std::ofstream ss;

    ss.open("Wind.kml");
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
          "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n"
          "<Document><name>Wind</name><Folder>\n";
    std::string color;
    std::string yellowArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/6/66/"
                              "Arrow-180%28ff0%29.svg/200px-Arrow-180%28ff0%29.svg.png</href>";
    std::string redArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/c/ce/"
                           "Arrow-180%28f00%29.svg/200px-Arrow-180%28f00%29.svg.png</href>";
    std::string greenArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/5/5b/"
                             "Arrow-180%28080%29.svg/200px-Arrow-180%28080%29.svg.png</href>";
    std::string orangeArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/0/03/"
                              "Arrow-180%28f80%29.svg/200px-Arrow-180%28f80%29.svg.png</href>";
    std::string whiteArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/f/f8/"
                             "Arrow-180%28fff%29.svg/200px-Arrow-180%28fff%29.svg.png</href>";

    for (size_t i = 0; i < angles[0].size(); i++) {
      double dist = sqrt(pow(lats[i]-lats[angles[0].size()-1], 2) + pow(lons[i]-lons[angles[0].size()-1], 2));
      int time_step;
      if (dist < 2) {
        time_step = 0;
      } else if (dist < 4) {
        time_step = 1;
      } else if (dist < 6) {
        time_step = 2;
      } else {
        time_step = 3;
      }

      int wind_speed = magnitudes[time_step][i];

      if (wind_speed < 6) {
        color = whiteArrow;
      } else if (wind_speed < 11) {
        color = yellowArrow;
      } else if (wind_speed < 16) {
        color = greenArrow;
      } else if (wind_speed < 21) {
        color = orangeArrow;
      } else {
        color = redArrow;
      }

      ss << "<GroundOverlay>"
            "<color>ffffffff</color>"
            "<drawOrder>1</drawOrder>"
            "<Icon>"
            << color <<
            "<refreshMode>onInterval</refreshMode>"
            "<refreshInterval>86400</refreshInterval>"
            "<viewBoundScale>0.75</viewBoundScale>"
            "</Icon>"
            "<LatLonBox>"
            "<north>" << lats[i] + magnitudes[time_step][i]/80 << "</north>"
            "<south>" << lats[i] - magnitudes[time_step][i]/80 << "</south>"
            "<east>" << lons[i] + magnitudes[time_step][i]/80 << "</east>"
            "<west>" << lons[i] - magnitudes[time_step][i]/80 << "</west>"
            "<rotation>" << 360-angles[time_step][i] << "</rotation>"
            "</LatLonBox>"
            "</GroundOverlay>" << std::endl;
    }
    ss << "</Folder>\n</Document>\n</kml>" << std::endl;

    ss.close();
}


void gribParse::saveToCsv2D(const std::vector<std::vector<double>> & array2D, const std::string & csvfilename) {
    std::ofstream outfile;
    outfile.open(csvfilename);

    for (size_t i = 0; i < array2D.size(); i++) {
        for (size_t j = 0; j < array2D.at(i).size(); j++) {
            outfile << array2D[i][j];
            if (static_cast<int>(j) < static_cast<int>(array2D.at(i).size()) - 1) {
                outfile << ",";
            } else {
                outfile << "\n";
            }
        }
    }
    outfile.close();
}

std::vector<std::vector<double>> gribParse::convert1Dto2D(const std::vector<double> & array1D,
                                                          int numRows, int numCols) {
    std::vector<std::vector<double>> array2D(numRows, std::vector<double>(numCols, 0));
    for (int row = 0; row < numRows; row++) {
      for (int col = 0; col < numCols; col++) {
        int index = row * numCols + col;
        array2D[row][col] = array1D.at(index);
      }
    }
    return array2D;
}

std::vector<double> gribParse::convert2Dto1D(const std::vector<std::vector<double>> & array2D) {
  std::vector<double> array1D;
  for (size_t i = 0; i < array2D.size(); i++) {
    for (size_t j = 0; j < array2D.at(i).size(); j++) {
      array1D.push_back(array2D.at(i).at(j));
    }
  }
  return array1D;
}

std::vector<std::vector<double>> gribParse::readCsv(const std::string & csvfilename) {
    // Read in raw csv lines
    std::ifstream infile(csvfilename);
    std::string line;
    std::vector<std::string> data;
    while (std::getline(infile, line)) {
        data.push_back(line);
    }

    // Parse csv lines
    std::vector<std::vector<double>> returnValue;
    for (size_t i = 0; i < data.size(); i++) {
        std::vector<double> row;

        // Continuously read data points until end of line reached
        std::string remaining_line = data.at(i);
        std::size_t pos = remaining_line.find(",");
        while (pos != std::string::npos) {
            // Get string before comma and convert to double
            std::string nextLine = remaining_line.substr(0, pos);
            double next = std::stod(nextLine);
            row.push_back(next);

            // Move to next element in the line
            remaining_line = remaining_line.substr(pos+1);  // Safe because 0 <= pos < remaining_line.size()
                                                            // then 0 < pos <= remaining_line.size() all valid
            pos = remaining_line.find(",");
        }

        // Handle last element. If last value is a ",", then will do nothing
        // If last value is a number like "10,11", then will add 11 here
        if (remaining_line.size() > 0) {
            row.push_back(std::stod(remaining_line));
        }

        returnValue.push_back(row);
    }
    return returnValue;
}

std::vector<std::vector<double>> gribParse::reverseColumns(const std::vector<std::vector<double>> & array2D) {
    // Reverse columns by changing the order of rows (each row content is same, but change order)
    std::vector<std::vector<double>> reversed;
    for (int row = array2D.size() - 1; row >= 0; row--) {
      reversed.push_back(array2D.at(row));
    }
    return reversed;
}
