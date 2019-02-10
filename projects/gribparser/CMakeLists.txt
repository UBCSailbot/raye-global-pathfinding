cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME} CXX)

set(PROJECT_NAME gribparser)

set(${PROJECT_NAME}_SRCS
        main.cpp
        gribParse.cpp
        gribParse.h
        )

set(${PROJECT_NAME}_LIBS src_core eccodes ${CURL_LIBRARIES})

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
