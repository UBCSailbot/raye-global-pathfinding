###########
# SOURCES #
###########

set(LIB_SRCS
        core/HexPlanet.cpp
        datatypes/HexTriangle.cpp
        datatypes/HexVertex.cpp)

set(LIB_HDRS
        core/HexPlanet.h
        datatypes/HexDefs.h
        datatypes/HexTriangle.h
        datatypes/HexVertex.h
        datatypes/MapData.h)

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
