// Copyright 2017 UBC Sailbot

#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/program_options.hpp>

#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/HaversineCostCalculator.h>
#include <pathfinding/WeatherCostCalculator.h>
#include <pathfinding/AStarPathfinder.h>
#include <pathfinding/PathfinderResultPrinter.h>
#include "pathfinding/WeatherHexMap.h"
#include <logic/StandardCalc.h>

#include "network_table_api/NonProtoConnection.h"
#include "network_table_api/Help.h"

constexpr uint8_t kInvalidIndirectNeighbourDepth = static_cast<uint8_t> (-1);

enum class OutputFormat {
  kDefault,
  kKML
};

double start_lat, start_lon, end_lat, end_lon;
int pointToPrint;
bool preserveKml = false;

void find_neighbours(const HexPlanet &planet, HexVertexId id) {
  std::cout << "Finding neighbours for vertex ID: " << id << std::endl;

  const HexVertex &vertex = planet.vertex(id);
  const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
  for (size_t i = 0; i < vertex.neighbour_count; i++) {
    std::cout << neighbours[i] << std::endl;
  }
}

void find_coordinates(HexPlanet &planet, const std::vector<HexVertexId> &ids) {
  std::cout << "Finding coordinates for vertices: <id> (<lat>, <lng>)" << std::endl;
  for (HexVertexId id : ids) {
    std::cout << id << " " << planet.vertex(id).coordinate.to_string() << std::endl;
  }
}

Pathfinder::Result run_pathfinder(HexPlanet &planet,
                                  HexVertexId source,
                                  HexVertexId target,
                                  int weather_factor,
                                  bool generate_new_grib,
                                  const std::string & file_name,
                                  int time_steps,
                                  bool use_csvs,
                                  const std::string & output_csvs_folder,
                                  bool silent,
                                  bool verbose) {
  HaversineHeuristic heuristic = HaversineHeuristic(planet);
  WeatherHexMap weather_map = WeatherHexMap(planet, time_steps, start_lat, start_lon, end_lat, end_lon, generate_new_grib, file_name, use_csvs, output_csvs_folder, preserveKml);
  auto wmap_pointer = std::make_unique<WeatherHexMap>(weather_map);
  WeatherCostCalculator cost_calculator = WeatherCostCalculator(planet, wmap_pointer, weather_factor);
  AStarPathfinder pathfinder(planet, heuristic, cost_calculator, source, target, true);

  if (!silent) {
    std::cout << "Pathfinding from " << source << " to " << target << std::endl;
  }
  auto start_time = std::chrono::system_clock::now();

  auto result = pathfinder.Run();

  if (!silent) {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << std::fixed
              << "Pathfinding Complete (" << elapsed_seconds.count() << "s)" << std::endl;

    if (verbose) {
      auto stats = pathfinder.stats();
      std::cout << std::fixed
                << "Closed Set: " << stats.closed_set_size << std::endl
                << "Open Set:   " << stats.open_set_size << " (on exit)" << std::endl;
    }

    std::cout << std::endl;
  }

  return result;
}

HexPlanet generate_planet(uint8_t subdivision_level, uint8_t indirect_neighbour_depth, bool silent, bool verbose, bool store_planet, bool use_cached_planet) {
  auto start_time = std::chrono::system_clock::now();
  const std::string path_to_cached_planet = "cached_planets/size_" + std::to_string(subdivision_level) + ".txt";
  if (!silent) {
    if (use_cached_planet) {
      std::cout << "Looking for cached planet at " << path_to_cached_planet << std::endl;
    }
    else {
      std::cout << "Generating HexPlanet of Size: " << static_cast<int>(subdivision_level) << std::endl;
    }
  }
  HexPlanet planet = (use_cached_planet) ? HexPlanet(path_to_cached_planet) :
                     (indirect_neighbour_depth != kInvalidIndirectNeighbourDepth) ?
                     HexPlanet(subdivision_level, indirect_neighbour_depth) :
                     HexPlanet(subdivision_level);

  if (!silent) {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << std::fixed
              << "Planet Generation Complete (" << elapsed_seconds.count() << "s)" << std::endl;

    if (verbose) {
      std::cout << std::fixed
                << "Vertices:  " << planet.vertex_count() << std::endl
                << "Triangles: " << planet.triangle_count() << std::endl;
    }

    std::cout << std::endl;
  }

  if (store_planet)
  {
    if (!silent) {
      std::cout << "Storing planet at " << path_to_cached_planet << std::endl;
    }
    planet.WriteToFile(path_to_cached_planet);
  }

  return planet;
}

int main(int argc, char const *argv[]) {
  try {
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen")
        ("v,verbose", "Verbose output")
        ("s,silent", "Silence useful output")
        ("grib", boost::program_options::value<std::string>(),
         "Relative path to grb file for weather info. If not given, uses new weather data download.")
        ("input_csvs", boost::program_options::value<std::string>(),
         "Relative path to folder from which to read in csvs as weather data.")
        ("output_csvs", boost::program_options::value<std::string>(),
         "Relative path to existing folder in which weather data csvs will be created")
        ("p,planet_size", boost::program_options::value<int>()->default_value(1), "Planet Size")
        ("w,weather_factor", boost::program_options::value<int>()->default_value(1500), "Weather Factor")
        ("n,neighbour", boost::program_options::value<HexVertexId>(), "Vertex to find neighbours")
        ("i,indirect", boost::program_options::value<int>(), "Indirect neighbour depth")
        ("t,time_steps", boost::program_options::value<int>()->default_value(4), "Max time steps for wind speed")
        ("c,coordinates",
         boost::program_options::value<std::vector<HexVertexId>>()->multitoken(),
         "Vertices for which to find GPS Coordinates")
        ("f,find_path",
         boost::program_options::value<std::vector<HexVertexId>>()->multitoken(),
         "<start> <end> Vertex IDs")
        ("table", "Connect to network table")
        ("navigate",
         boost::program_options::value<std::vector<double>>()->multitoken(),
         "<start_latitude> <start_longitude> <end_latitude> <end_longitude>")
        ("kml", "Output the a KML file for the pathfinding result")
        ("store_planet", "Output the a file to store the planet as a cache")
        ("use_cached_planet", "Use cached_planet in cached_planets/size_<size>.txt")
        ("printn", boost::program_options::value<int>(), "Output the nth coordinate pair at the end of the program, starting with 1")
        ("save", "Save the current weather as a timestamped KML")
        ("hardcoded", boost::program_options::value<std::string>(), "Default use: --hardcoded {Month}, {Month} = Oct, Nov, Dec etc.");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") || vm.size() <= 1) {
      // Show help if no non-default args are passed (p is always defined).
      std::cout << desc;
      return EXIT_FAILURE;
    }

    bool generate_new_grib = true;
    bool use_csvs = false;
    std::string file_name = "data.grb";
    std::string output_csvs_folder = (vm.count("output_csvs")) ? vm["output_csvs"].as<std::string>() : "";
    if (vm.count("grib")) {
      generate_new_grib = false;
      file_name = vm["grib"].as<std::string>();
      use_csvs = false;
    } else if (vm.count("input_csvs")) {
      generate_new_grib = false;
      file_name = vm["input_csvs"].as<std::string>();
      use_csvs = true;
    }

    bool silent = vm.count("s") > 0;
    OutputFormat format = OutputFormat::kDefault;

    if (vm.count("kml") > 0) {
      silent = true;
      format = OutputFormat::kKML;
    }

    if (vm.count("printn") > 0) {
      pointToPrint = (vm["printn"].as<int>());
    }

    else {
      pointToPrint = 0;
    }

    if (vm.count("save") > 0) {
      preserveKml = true;
    }

    bool verbose = vm.count("v") > 0 && !silent;

    uint8_t planet_size = static_cast<uint8_t> (vm["p"].as<int>());
    uint8_t indirect_neighbour_depth = (vm.count("i") > 0) ? static_cast<uint8_t> (vm["i"].as<int>())
                                                           : kInvalidIndirectNeighbourDepth;
    const bool store_planet = vm.count("store_planet") > 0;
    const bool use_cached_planet = vm.count("use_cached_planet") > 0;
    HexPlanet planet = generate_planet(planet_size, indirect_neighbour_depth, silent, verbose, store_planet, use_cached_planet);

    int time_steps = vm["t"].as<int>();

    int weather_factor = vm["w"].as<int>() * std::pow(2,10-planet_size);

    if (vm.count("n")) {
      find_neighbours(planet, vm["n"].as<HexVertexId>());
    } else if (vm.count("c")) {
      find_coordinates(planet, vm["c"].as<std::vector<HexVertexId>>());
    } else if (vm.count("f")) {
      auto points = vm["f"].as<std::vector<HexVertexId>>();

      if (points.size() != 2) {
        throw std::runtime_error("Pathfinding requires two hex IDs: <start> <end>");
      }

      auto result = run_pathfinder(planet, points[0], points[1], weather_factor, generate_new_grib, file_name,
                                   time_steps, use_csvs, output_csvs_folder, silent, verbose);

      switch (format) {
        case OutputFormat::kDefault:
          // Print default
          std::cout << PathfinderResultPrinter::PrintDefault(result);

          if (verbose) {
            std::cout << std::endl;
            std::cout << PathfinderResultPrinter::PrintCoordinates(planet, result);
          }
          break;
        case OutputFormat::kKML:
          // Print KML
          std::cout << PathfinderResultPrinter::PrintKML(planet, result, weather_factor, file_name, time_steps, use_csvs, output_csvs_folder, pointToPrint, preserveKml, false);
          break;
      }
    } else if (vm.count("navigate")) {
      // Find a path betweeen two GPS coordinates, print in KML format
      //TODO() Enable Inputs to be in degrees West/South
      auto points = vm["navigate"].as<std::vector<double>>();

      NetworkTable::NonProtoConnection connection;
      if (vm.count("table")) {
        // Connect to the network table
        int error_count = 0;
        double last_lat = 0.0, last_lon = 0.0;
        std::ifstream read_handle ("LastCoords.txt");
        std::string last_coords;
        std::string error_msg;

        if (read_handle.is_open()) {
          getline(read_handle, last_coords);
          int space_index = last_coords.find(" ");
          last_lon = std::stod(last_coords.substr(0, space_index));
          last_lat = std::stod(last_coords.substr(space_index + 1, last_coords.length()));
          read_handle.close();
        }

        try {
            std::cout << "Connecting to network table" <<std::endl;
            connection.Connect(100);
        } catch (NetworkTable::TimeoutException) {
            std::cout << "Failed to connect" << std::endl;
            ++error_count;
        }

        double gps_lat, gps_lon;
        try {
            gps_lat = (double) connection.GetNode("/gps_can/gprmc/latitude").value().float_data();
            gps_lon = (double) connection.GetNode("/gps_can/gprmc/longitude").value().float_data();
            if (gps_lat == 0.0 || gps_lon == 0.0) {
              error_msg = "GPS Coords cannot be zero";
              ++error_count;
            }
            else if (gps_lon == last_lon || gps_lat == last_lat) {
              error_msg = "GPS Coords have not updated";
              ++error_count;
            }
        } catch (NetworkTable::NodeNotFoundException ex) {
            connection.Disconnect();
            error_msg = "Gps Coords Not Found in Network Table";
            ++error_count;
        }

        if (error_count == 0) {
          start_lat = gps_lat;
          start_lon = gps_lon;

          std::ofstream write_handle ("LastCoords.txt");
          if (write_handle.is_open()) {
            write_handle << gps_lon << " " << gps_lat << std::endl;
            write_handle.close();
          }

        } else {
          throw (std::runtime_error(error_msg));
        }

      } else {
        start_lat = (points[0]);
        start_lon = (points[1]);
      }

      end_lat = (points[2]);
      end_lon = (points[3]);

      auto adj_start_lon = start_lon < 0 ? start_lon : start_lon - 360;
      auto adj_end_lon = end_lon < 0 ? end_lon : end_lon - 360;

      const GPSCoordinateFast start_coord(start_lat, adj_start_lon, true);
      const GPSCoordinateFast end_coord(end_lat, adj_end_lon, true);

      Eigen::Vector3f start_point;
      Eigen::Vector3f end_point;

      standard_calc::CoordToPoint(start_coord, &start_point);
      standard_calc::CoordToPoint(end_coord, &end_point);

      HexVertexId start_vertex = planet.HexVertexFromPoint(start_point);
      HexVertexId end_vertex = planet.HexVertexFromPoint(end_point);

      auto result = run_pathfinder(planet, start_vertex, end_vertex, weather_factor, generate_new_grib, file_name,
                                   time_steps, use_csvs, output_csvs_folder, silent, verbose);

      std::vector<std::pair<double, double>> waypoints;

      if (vm.count("hardcoded")) {
        /*
       try{
         waypoints = PathfinderResultPrinter::GetHardcoded(vm["hardcoded"].as<std::string>());
         std::cout << PathfinderResultPrinter::PrintHardcoded(waypoints);
       }
       catch(std::string test_name){
         std::cout << "Error: " << test_name << " is not a valid hardcoded test" << std::endl;
       }*/
        waypoints = PathfinderResultPrinter::GetVector(planet, result, true);
      } else {
        waypoints = PathfinderResultPrinter::GetVector(planet, result, false);
      }

      if (vm.count("table")) {
        try {
          connection.SetWaypointValues(waypoints);
          connection.Disconnect();
        }
        catch(NetworkTable::TimeoutException){
          std::cout << "Could not set waypoint values" << std::endl;
        }
      } if (vm.count("hardcoded")) {
        std::cout << PathfinderResultPrinter::PrintKML(planet, result, weather_factor, file_name, time_steps, use_csvs, output_csvs_folder, pointToPrint, preserveKml, true);
      } else {
        std::cout << PathfinderResultPrinter::PrintKML(planet, result, weather_factor, file_name, time_steps, use_csvs, output_csvs_folder, pointToPrint, preserveKml, false);
      }

    } else {
      std::cerr << "Invalid Program options." << std::endl
                << desc;
      return EXIT_FAILURE;
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  } catch (const boost::bad_any_cast &ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  } catch (const std::runtime_error &ex) {
    std::cerr << "Pathfinding Error:" << std::endl;
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
