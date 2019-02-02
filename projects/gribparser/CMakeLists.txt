cmake_minimum_required(VERSION 3.12)
project(grib_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories()

add_executable(grib_parser
        CMakeLists.txt
        main.cpp
        urlBuilder.cpp
        urlBuilder.h
        urlDownloader.cpp
        urlDownloader.h
        gribParse.cpp
        gribParse.h
        )

target_link_libraries(grib_parser curl)


find_package(eccodes REQUIRED)
include_directories(${ECCODES_INCLUDE_DIRS})

target_link_libraries(grib_parser
        eccodes        )
