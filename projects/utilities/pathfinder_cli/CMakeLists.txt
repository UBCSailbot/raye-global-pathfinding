# Set a variable for commands below
set(PROJECT_NAME pathfinder_cli)

# Define your project and language
project(${PROJECT_NAME} CXX)

# Define the source code
set(${PROJECT_NAME}_SRCS main.cpp)

# Depending on your CMake & Boost versions, this may throw a warning but will likely work regardless.
find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(network_table_api)
set(${PROJECT_NAME}_LIBS src_core ${Boost_LIBRARIES})

target_compile_definitions(src_core PUBLIC WELCOME_DIRECTORY="/tmp/sailbot")

include(FindPkgConfig)
pkg_search_module(ZMQ REQUIRED libzmq)
include_directories(${ZMQ_INCLUDE_DIRS})
 
add_library(libnt_client STATIC IMPORTED)
set_target_properties(libnt_client PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/network_table_api/libnt_client.a)

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS} libnt_client ${ZMQ_LIBRARIES})
