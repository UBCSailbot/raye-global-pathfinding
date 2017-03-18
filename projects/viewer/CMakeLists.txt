# Set a variable for commands below
set(PROJECT_NAME viewer)

# Define your project and language
project(${PROJECT_NAME} CXX)

find_package(glfw3 REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

if (NOT APPLE)
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# Define the source code
set(${PROJECT_NAME}_SRCS
        ArcBall.cpp
        ArcBall.h
        Camera.cpp
        Camera.h
        ColourUtils.h
        Controls.cpp
        Controls.h
        EdgeRenderer.cpp
        EdgeRenderer.h
        main.cpp
        PlanetRenderer.cpp
        PlanetRenderer.h
        Program.cpp
        Program.h
        Shader.cpp
        Shader.h)

set(${PROJECT_NAME}_LIBS src_core ${glfw3_LIBRARY} ${OPENGL_gl_LIBRARY})

if (NOT APPLE)
    set(${PROJECT_NAME}_LIBS ${${PROJECT_NAME}_LIBS} ${GLEW_LIBRARIES})
endif()

# Define the executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS} EdgeRenderer.cpp PlanetRenderer.h ColourUtils.h EdgeUtils.cpp EdgeUtils.h)
target_link_libraries(${PROJECT_NAME} glfw ${${PROJECT_NAME}_LIBS})
