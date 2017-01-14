# Set a variable for commands below
set(PROJECT_NAME example)

# Define your project and language
project(${PROJECT_NAME} CXX)

# Define the source code
set(${PROJECT_NAME}_SRCS main.cpp)
set(${PROJECT_NAME}_LIBS src_core)

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
