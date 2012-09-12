cmake_minimum_required(VERSION 2.8)
project(luabind)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )

set( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/pc CACHE PATH
        "Single Directory for all Libraries")

# we require boost. headers only for luabind
find_package(Boost 1.51.0)

# all source files
file(GLOB_RECURSE LUABIND_SRC src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} src ../lua/src)

# compile the static lib
add_library(luabind STATIC ${LUABIND_SRC})
