// Copyright 2017 UBC Sailbot

#include <iostream>

#include <boost/program_options.hpp>

#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/NaiveCostCalculator.h>
#include <pathfinding/AStarPathfinder.h>

void find_neighbors(HexVertexId id, const HexPlanet &planet) {
  std::cout << "Neighbor Find for ID: " << id << std::endl;
  const HexVertex &vertex = planet.vertex(id);
  const std::array<HexVertexId, 6> &neighbours = vertex.neighbours;
  for (size_t i = 0; i < vertex.neighbour_count; i++) {
    std::cout << neighbours[i] << std::endl;
  }
  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {
  try {
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen")
        ("p,planet_size", boost::program_options::value<int>()->default_value(1), "Planet Size")
        ("f,find_path", boost::program_options::value<std::vector<HexVertexId>>()->multitoken(), "Pathfinding coordinates")
        ("n,neighbor", boost::program_options::value<HexVertexId>(), "Vertex to find neighbors");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    HexPlanet planet(vm["p"].as<int>());

    if (vm.count("help") || vm.size() == 0) {
      // Show help
      std::cout << desc;
      return EXIT_SUCCESS;
    }

    if (vm.count("p")) {
      std::cout << "Planet Size: " << vm["p"].as<int>() << std::endl;
    }

    if (vm.count("n")) {
      find_neighbors(vm["n"].as<HexVertexId>(), planet);
    } else if (vm.count("f") && (vm["f"].as<std::vector<HexVertexId> >().size() == 2)) {
      HaversineHeuristic heuristic = HaversineHeuristic();
      NaiveCostCalculator cost_calculator = NaiveCostCalculator(planet);
      auto points = vm["f"].as<std::vector<HexVertexId>>();
      AStarPathfinder pathfinder(planet, &heuristic, &cost_calculator, points[0], points[1]);
      auto path = pathfinder.Run();

      std::cout << "Path: " << std::endl;
      for (auto point : path) {
        std::cout << point << " ";
      }
      std::cout << std::endl;
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << std::endl;
  } catch (const std::runtime_error &ex) {
    std::cerr << "Pathfinding Error:" << std::endl;
    std::cerr << ex.what() << std::endl;
  }
}
