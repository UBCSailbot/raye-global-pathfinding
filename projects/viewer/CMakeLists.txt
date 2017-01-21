# Set a variable for commands below
set(PROJECT_NAME viewer)

# Define your project and language
project(${PROJECT_NAME} CXX)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Define the source code
set(${PROJECT_NAME}_SRCS
        main.cpp
        Shader.cpp
        Shader.h
        Controls.cpp
        Controls.h
        Camera.cpp
        Camera.h
        ArcBall.cpp
        ArcBall.h
        Program.cpp
        Program.h)

set(${PROJECT_NAME}_LIBS src_core ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
