###########
# SOURCES #
###########

set(LIB_SRCS
        example/Example.cpp
        # Add new source files here
        )

set(LIB_HDRS
        example/Example.h
        # Add new header files here
        )

# Define the src_core library target
add_library(src_core STATIC ${LIB_SRCS} ${LIB_HDRS})

# Link dependencies
add_dependencies(src_core protofiles)
