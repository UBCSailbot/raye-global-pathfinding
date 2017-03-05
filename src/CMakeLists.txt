###########
# SOURCES #
###########

set(LIB_SRCS
        datatypes/GPSCoordinate.cpp
        datatypes/GPSCoordinateFast.cpp
        datatypes/HexTriangle.cpp
        datatypes/HexVertex.cpp
        logic/StandardCalc.cpp
        pathfinding/AStarPathfinder.cpp
        pathfinding/AStarVertex.cpp
        pathfinding/HaversineHeuristic.cpp
        pathfinding/NaiveCostCalculator.cpp
        pathfinding/NaiveHeuristic.cpp
        pathfinding/Pathfinder.cpp
        planet/HexPlanet.cpp)

set(LIB_HDRS
        common/GeneralDefs.h
        datatypes/GPSCoordinate.h
        datatypes/GPSCoordinateFast.h
        datatypes/HexDefs.h
        datatypes/HexTriangle.h
        datatypes/HexVertex.h
        datatypes/MapData.h
        logic/StandardCalc.h
        pathfinding/AStarPathfinder.h
        pathfinding/AStarVertex.h
        pathfinding/CostCalculator.h
        pathfinding/HaversineHeuristic.h
        pathfinding/Heuristic.h
        pathfinding/NaiveCostCalculator.h
        pathfinding/NaiveHeuristic.h
        pathfinding/Pathfinder.h
        planet/HexPlanet.h)

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
