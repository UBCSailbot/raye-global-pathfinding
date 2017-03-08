// Copyright 2017 UBC Sailbot

#include "pathfinding/PathfinderResultPrinter.h"

#include <iomanip>

std::string PathfinderResultPrinter::PrintDefault(const Pathfinder::Result &result) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(1)
     << "Cost: " << result.cost << " (distance in meters)" << std::endl
     << "Time: " << result.time << std::endl;

  ss << "Path: " << std::endl;
  for (auto point : result.path) {
    ss << point << " ";
  }
  ss << std::endl;

  return ss.str();
}

std::string PathfinderResultPrinter::PrintCoordinates(HexPlanet &planet, const Pathfinder::Result &result) {
  std::stringstream ss;

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    ss << id << ", " << coord.to_string_latitude() << ", " << coord.to_string_longitude() << std::endl;
  }

  return ss.str();
}

std::string PathfinderResultPrinter::PrintKML(HexPlanet &planet, const Pathfinder::Result &result) {
  std::stringstream ss;
  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    ss << coord.to_string_longitude() << ", " << coord.to_string_latitude() << std::endl;
  }

  ss << "</coordinates></LineString></Placemark></Document></kml>\n";

  return ss.str();
}
