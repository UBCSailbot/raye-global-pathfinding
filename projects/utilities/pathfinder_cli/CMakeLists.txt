# Set a variable for commands below
set(PROJECT_NAME pathfinder_cli)

# Define your project and language
# project(${PROJECT_NAME} CXX)
project(${PROJECT_NAME})

# Define the source code
set(${PROJECT_NAME}_SRCS main.cpp)

# Depending on your CMake & Boost versions, this may throw a warning but will likely work regardless.
find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
set(${PROJECT_NAME}_LIBS src_core ${Boost_LIBRARIES})

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS} ${catkin_LIBRARIES})
