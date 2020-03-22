// Copyright 2017 UBC Sailbot

#include <chrono>
#include <iostream>
#include <iomanip>

#include "ros/ros.h"
#include "local_pathfinding/path.h"
#include "local_pathfinding/latlon.h"
#include "local_pathfinding/GPS.h"
#include <vector>
#include <stdlib.h>

#include <boost/program_options.hpp>

#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/HaversineCostCalculator.h>
#include <pathfinding/WeatherCostCalculator.h>
#include <pathfinding/AStarPathfinder.h>
#include <pathfinding/PathfinderResultPrinter.h>

#include <logic/StandardCalc.h>

constexpr uint8_t kInvalidIndirectNeighbourDepth = static_cast<uint8_t> (-1);

enum class OutputFormat {
  kDefault,
  kKML
};

int start_lat, start_lon, end_lat, end_lon;

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
                                  bool silent,
                                  bool verbose) {
  HaversineHeuristic heuristic = HaversineHeuristic(planet);
  HaversineCostCalculator h_cost_calculator = HaversineCostCalculator(planet);
  WeatherHexMap weather_map = WeatherHexMap(planet, 4, start_lat, start_lon, end_lat, end_lon);
  auto wmap_pointer = std::make_unique<WeatherHexMap>(weather_map);
  WeatherCostCalculator cost_calculator = WeatherCostCalculator(planet, wmap_pointer);
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

HexPlanet generate_planet(uint8_t subdivision_level, uint8_t indirect_neighbour_depth, bool silent, bool verbose) {
  if (!silent) {
    std::cout << "Generating HexPlanet of Size: " << static_cast<int> (subdivision_level) << std::endl;
  }
  auto start_time = std::chrono::system_clock::now();

  HexPlanet planet = indirect_neighbour_depth != kInvalidIndirectNeighbourDepth ? HexPlanet(subdivision_level,
                                                                                            indirect_neighbour_depth)
                                                                                : HexPlanet(subdivision_level);

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

  return planet;
}

local_pathfinding::latlon createLatlon(double lat, double lon) {
  local_pathfinding::latlon newlatlon;
  newlatlon.lat = lat;
  newlatlon.lon = lon;
  return newlatlon;
}

// Global variable for tracking boat position
local_pathfinding::latlon sailbot_position;

void gpsCallback(const local_pathfinding::GPS& msg)
{
  sailbot_position.lat = msg.lat;
  sailbot_position.lon = msg.lon;
}


int main(int argc, char *argv[]) {
  try {
    sailbot_position.lat = 48.5;
    sailbot_position.lon = -124.8;
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen")
        ("v,verbose", "Verbose output")
        ("s,silent", "Silence useful output")
        ("p,planet_size", boost::program_options::value<int>()->default_value(1), "Planet Size")
        ("n,neighbour", boost::program_options::value<HexVertexId>(), "Vertex to find neighbours")
        ("i,indirect", boost::program_options::value<int>(), "Indirect neighbour depth")
        ("c,coordinates",
         boost::program_options::value<std::vector<HexVertexId>>()->multitoken(),
         "Vertices for which to find GPS Coordinates")
        ("f,find_path",
         boost::program_options::value<std::vector<HexVertexId>>()->multitoken(),
         "<start> <end> Vertex IDs")
	("navigate",
	 boost::program_options::value<std::vector<double>>()->multitoken(),
	 "<start_latitude> <start_longitude> <end_latitude> <end_longitude>")
        ("kml", "Output the a KML file for the pathfinding result");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") || vm.size() <= 1) {
      // Show help if no non-default args are passed (p is always defined).
      std::cout << desc;
      return EXIT_FAILURE;
    }

    bool silent = vm.count("s") > 0;
    OutputFormat format = OutputFormat::kDefault;

    if (vm.count("kml") > 0) {
      silent = true;
      format = OutputFormat::kKML;
    }

    bool verbose = vm.count("v") > 0 && !silent;

    uint8_t planet_size = static_cast<uint8_t> (vm["p"].as<int>());
    uint8_t indirect_neighbour_depth = (vm.count("i") > 0) ? static_cast<uint8_t> (vm["i"].as<int>())
                                                           : kInvalidIndirectNeighbourDepth;
    HexPlanet planet = generate_planet(planet_size, indirect_neighbour_depth, silent, verbose);


    if (vm.count("n")) {
      find_neighbours(planet, vm["n"].as<HexVertexId>());
    } else if (vm.count("c")) {
      find_coordinates(planet, vm["c"].as<std::vector<HexVertexId>>());
    } else if (vm.count("f")) {
      auto points = vm["f"].as<std::vector<HexVertexId>>();

      if (points.size() != 2) {
        throw std::runtime_error("Pathfinding requires two hex IDs: <start> <end>");
      }

      auto result = run_pathfinder(planet, points[0], points[1], silent, verbose);

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
          std::cout << PathfinderResultPrinter::PrintKML(planet, result);
          break;
      }
    } else if (vm.count("navigate")) {
      // Find a path betweeen two GPS coordinates, print in KML format
      //TODO() Enable Inputs to be in degrees West/South
      auto points = vm["navigate"].as<std::vector<double>>();

      start_lat = (points[0]);
      start_lon = (points[1]);
      end_lat = (points[2]);
      end_lon = (points[3]);

      auto adj_start_lon = start_lon < 0 ? start_lon : start_lon - 360;
      auto adj_end_lon = end_lon < 0 ? end_lon : end_lon - 360;

      const GPSCoordinateFast start_coord(start_lat*10000000, adj_start_lon*10000000);
      const GPSCoordinateFast end_coord(end_lat*10000000, adj_end_lon*10000000);

      Eigen::Vector3f start_point;
      Eigen::Vector3f end_point;

      standard_calc::CoordToPoint(start_coord, &start_point);
      standard_calc::CoordToPoint(end_coord, &end_point);

      HexVertexId start_vertex = planet.HexVertexFromPoint(start_point);
      HexVertexId end_vertex = planet.HexVertexFromPoint(end_point);

      auto result = run_pathfinder(planet, start_vertex, end_vertex, silent, verbose);
      std::cout << PathfinderResultPrinter::PrintKML(planet, result) << std::endl;

      // ROS init code
      std::cout << "STARTING ROS LOOP" << std::endl;
      ros::init(argc, argv, "global_pathfinding_node");
      ros::NodeHandle n;
      ros::Subscriber sub = n.subscribe("GPS", 100, gpsCallback);
      ros::Publisher chatter_pub = n.advertise<local_pathfinding::path>("globalPath", 10);
      double publish_period_seconds = 10;
      ros::Rate loop_rate(1.0 / publish_period_seconds);
      while (ros::ok()) {
        // Create dummy vector of latlons
        int num_waypoints = 50;
        std::vector<local_pathfinding::latlon> mywaypoints(num_waypoints + 1);
        local_pathfinding::latlon first = createLatlon(sailbot_position.lat, sailbot_position.lon);
        local_pathfinding::latlon last = createLatlon(20.0, -156.0);
        for (int i = 0; i <= num_waypoints; i++) {
          double coefficient = ((double)(i)) / num_waypoints;
          double lat = (1 - coefficient) * first.lat + coefficient * last.lat;
          double lon = (1 - coefficient) * first.lon + coefficient * last.lon;
          local_pathfinding::latlon mylatlon = createLatlon(lat, lon);
          mywaypoints[i] = mylatlon;
        }

        // Create path msg
        local_pathfinding::path msg;
        msg.waypoints = mywaypoints;

        ROS_INFO("global pathfinding node publishing");
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
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
