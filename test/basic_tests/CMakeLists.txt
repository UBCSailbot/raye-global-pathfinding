include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
        datatypes/GPSCoordinateTest.cpp
        datatypes/HexVertexTest.cpp
        grib/FileParseWindTest.cpp
        logic/StandardCalcTest.cpp
        pathfinding/AStarPathfinderTest.cpp
        pathfinding/BasicCostCalculatorTest.cpp
        pathfinding/BasicHexMapTest.cpp
        pathfinding/MockCostCalculator.cpp
        pathfinding/WeatherCostCalculatorTest.cpp
        pathfinding/WeatherHexMapTest.cpp
        planet/HexPlanetTest.cpp)

include_directories(.)
add_executable(run_basic_tests ${TEST_FILES})

target_link_libraries(run_basic_tests gtest gtest_main)
target_link_libraries(run_basic_tests src_core)
