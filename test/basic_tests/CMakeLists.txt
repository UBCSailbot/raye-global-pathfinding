include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
        datatypes/HexVertexTest.cpp
        logic/StandardCalcTest.cpp
        planet/HexPlanetTest.cpp
        pathfinding/AStarPathfinderTest.cpp
        pathfinding/MockCostCalculator.cpp)

include_directories(.)
add_executable(run_basic_tests ${TEST_FILES})

target_link_libraries(run_basic_tests gtest gtest_main)
target_link_libraries(run_basic_tests src_core)
