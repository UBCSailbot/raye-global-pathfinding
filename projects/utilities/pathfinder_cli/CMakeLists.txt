# Set a variable for commands below
set(PROJECT_NAME pathfinder_cli)

# Define your project and language
project(${PROJECT_NAME} CXX)

# Define the source code
set(${PROJECT_NAME}_SRCS main.cpp)
set(${PROJECT_NAME}_LIBS src_core)

find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} Boost::program_options ${${PROJECT_NAME}_LIBS})
