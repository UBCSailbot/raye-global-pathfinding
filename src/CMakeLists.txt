###########
# SOURCES #
###########

set(LIB_SRCS
        core/HexPlanet.cpp
        example/Example.cpp
        datatypes/Node.cpp
        datatypes/HexTriangle.h
        datatypes/HexVertex.cpp
        datatypes/HexTriangle.cpp)

set(LIB_HDRS
        core/HexPlanet.h
        example/Example.h
        datatypes/Node.h
        datatypes/MapData.h
        datatypes/HexTriangle.h
        datatypes/HexVertex.h)

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
