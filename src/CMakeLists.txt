###########
# SOURCES #
###########

set(LIB_SRCS
        core/HexPlanet.cpp
        datatypes/HexTriangle.cpp
        datatypes/HexVertex.cpp
        datatypes/Node.cpp
        example/Example.cpp)

set(LIB_HDRS
        core/HexPlanet.h
        datatypes/HexTriangle.h
        datatypes/HexVertex.h
        datatypes/MapData.h
        datatypes/Node.h
        example/Example.h)

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
