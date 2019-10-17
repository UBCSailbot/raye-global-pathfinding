// Copyright 2017 UBC Sailbot

#include "pathfinding/PathfinderResultPrinter.h"

#include <iomanip>
#include <fstream>

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
  std::ofstream handle;
  std::stringstream ss;

  // Throw an exception on failure to open the file or on a write error.
  handle.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  handle.open("Sample.kml");

  handle << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<kml xmlns=\"http://earth.google.com/kml/2.0\"><Document><Placemark><LineString><coordinates>\n";

  for (HexVertexId id : result.path) {
    const auto &coord = planet.vertex(id).coordinate;
    handle << coord.to_string_longitude() << "," << coord.to_string_latitude() << std::endl;
    ss << coord.to_string_longitude() << "," << coord.to_string_latitude() << std::endl;
  }

  handle << "</coordinates></LineString></Placemark></Document></kml>\n";

  ss << "</coordinates></LineString></Placemark></Document></kml>\n";

  handle.close();

  return ss.str();
}
