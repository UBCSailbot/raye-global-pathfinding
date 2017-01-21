###########
# SOURCES #
###########

set(LIB_SRCS
        core/hexplanet.cpp
        example/Example.cpp
        datatypes/Node.cpp
        datatypes/HexTri.h
        datatypes/HexTile.cpp
        datatypes/HexTri.cpp)

set(LIB_HDRS
        core/hexplanet.h
        example/Example.h
        datatypes/Node.h
        datatypes/MapData.h
        datatypes/HexTri.h
        datatypes/HexTile.h)

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
