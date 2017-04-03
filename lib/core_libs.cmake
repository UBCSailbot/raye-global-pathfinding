#################
# CORE LIBRARIES #
#################

set(CORE_LIBS)

list(APPEND CORE_LIBS ${CMAKE_THREAD_LIBS_INIT})

# Generates source for shared message data types using protobuf
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/protofiles)
include_directories(${ProtobufIncludePath})
list(APPEND CORE_LIBS protofiles)

find_package(Boost 1.58 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

list(APPEND CORE_LIBS ${Boost_LIBRARIES})

# Disable building extras we won't need (pure C++ project)
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# Add the configurations from nanogui
add_subdirectory(lib/nanogui)
include_directories(lib/nanogui/include)

# For reliability of parallel build, make the NanoGUI targets dependencies
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")