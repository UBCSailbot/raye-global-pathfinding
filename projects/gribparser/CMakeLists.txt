cmake_minimum_required(VERSION 3.12)
project(grib_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(grib_parser
        CMakeLists.txt
        main.cpp
        urlBuilder.cpp
        urlBuilder.h
        urlDownloader.cpp
        urlDownloader.h
        StandardCalc.cpp
        StandardCalc.h fileParse.cpp fileParse.h)

target_link_libraries(grib_parser curl)
set ( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ~/cpp-netlib-build )


find_package(eccodes REQUIRED)
include_directories(${ECCODES_INCLUDE_DIRS})

target_link_libraries(grib_parser
        eccodes        )