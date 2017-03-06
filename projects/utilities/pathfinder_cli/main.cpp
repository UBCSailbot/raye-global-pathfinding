// Copyright 2017 UBC Sailbot

#include <chrono>
#include <iostream>
#include <iomanip>

#include <boost/program_options.hpp>

#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/HaversineCostCalculator.h>
#include <pathfinding/AStarPathfinder.h>
#include <pathfinding/PathfinderResultPrinter.h>

enum class OutputFormat {
  kDefault,
  kKML
};

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
    auto coord = planet.GPSCoordinateFromHexVertex(id);
    std::cout << id << " " << coord.to_string() << std::endl;
  }
}

Pathfinder::Result run_pathfinder(HexPlanet &planet, HexVertexId source, HexVertexId target, bool silent) {
  HaversineHeuristic heuristic = HaversineHeuristic(planet);
  HaversineCostCalculator cost_calculator = HaversineCostCalculator(planet);
  AStarPathfinder pathfinder(planet, heuristic, cost_calculator, source, target);

  if (!silent) {
    std::cout << "Pathfinding from " << source << " to " << target << std::endl;
  }
  auto start_time = std::chrono::system_clock::now();

  auto result = pathfinder.Run();

  if (!silent) {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << std::fixed
        << "Pathfinding Complete (" << elapsed_seconds.count() << "s)" << std::endl << std::endl;
  }

  return result;
}

HexPlanet generate_planet(int subdivision_level, bool silent) {
  if (!silent) {
    std::cout << "Generating HexPlanet of Size: " << subdivision_level << std::endl;
  }
  auto start_time = std::chrono::system_clock::now();

  HexPlanet planet(subdivision_level);

  if (!silent) {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << std::fixed
        << "Planet Generation Complete (" << elapsed_seconds.count() << "s)" << std::endl << std::endl;
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
        ("p,planet_size", boost::program_options::value<int>()->default_value(1), "Planet Size")
        ("n,neighbour", boost::program_options::value<HexVertexId>(), "Vertex to find neighbours")
        ("c,coordinates",
         boost::program_options::value<HexVertexId>()->multitoken(),
         "Vertexes for which to find GPS Coordinates")
        ("f,find_path",
         boost::program_options::value<std::vector<HexVertexId>>()->multitoken(),
         "<start> <end> Vertex IDs")
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

    auto planet_size = vm["p"].as<int>();
    HexPlanet planet = generate_planet(planet_size, silent);

    if (vm.count("n")) {
      find_neighbours(planet, vm["n"].as<HexVertexId>());
    } else if (vm.count("c")) {
      find_coordinates(planet, vm["c"].as<std::vector<HexVertexId>>());
    } else if (vm.count("f")) {
      auto points = vm["f"].as<std::vector<HexVertexId>>();

      if (points.size() != 2) {
        throw std::runtime_error("Pathfinding requires two hex IDs: <start> <end>");
      }

      auto result = run_pathfinder(planet, points[0], points[1], silent);

      switch (format) {
        case OutputFormat::kDefault:
          std::cout << PathfinderResultPrinter::PrintDefault(result);

          if (verbose) {
            std::cout << std::endl;
            std::cout << PathfinderResultPrinter::PrintCoordinates(planet, result);
          }
          break;
        case OutputFormat::kKML:
          std::cout << PathfinderResultPrinter::PrintKML(planet, result);
          break;
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
