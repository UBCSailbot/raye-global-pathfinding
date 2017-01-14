cmake_minimum_required(VERSION 2.8)
project(base  C CXX)  # TODO: Change name

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O3 -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra")

############
# INCLUDES #
############

# Include libraries
include(lib/core_libs.cmake)

#########
# FLAGS #
#########

# Define the command-line flags that should be accessible to programs
option(SAILBOT_DEBUG "Whether to build the project with the SAILBOT_DEBUG variable defined at the preprocessor level" OFF)
if(SAILBOT_DEBUG)
  add_definitions(-DSAILBOT_DEBUG)
endif()

############
# PROGRAMS #
############

# Include library files to avoid ugly relative paths
include_directories(src)

# Add main library
add_subdirectory(src)

# Add projects (all executables)
add_subdirectory(projects)

# Add tests
add_subdirectory(test)
