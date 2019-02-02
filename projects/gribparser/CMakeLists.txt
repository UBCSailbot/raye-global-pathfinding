cmake_minimum_required(VERSION 3.12)
project(${PROJECT_NAME} CXX)

set(PROJECT_NAME gribparser)

set(${PROJECT_NAME}_SRCS
        main.cpp
        urlBuilder.cpp
        urlBuilder.h
        urlDownloader.cpp
        urlDownloader.h
        gribParse.cpp
        gribParse.h
        )

find_package(eccodes REQUIRED)
include_directories(${ECCODES_INCLUDE_DIRS})

set(${PROJECT_NAME}_LIBS src_core ${Boost_LIBRARIES} eccodes curl)

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
