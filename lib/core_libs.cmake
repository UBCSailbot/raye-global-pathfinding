#################
# CORE LIBRARIES #
#################

set( CORE_LIBS )

list(APPEND CORE_LIBS ${CMAKE_THREAD_LIBS_INIT})

# Generates source for shared message data types using protobuf
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/protofiles)
include_directories(${ProtobufIncludePath})
list(APPEND CORE_LIBS protofiles)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

list(APPEND CORE_LIBS ${Boost_LIBRARIES})
