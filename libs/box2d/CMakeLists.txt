cmake_minimum_required(VERSION 2.8)
project(box2d)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )

set( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/pc CACHE PATH
        "Single Directory for all Libraries")

# all source files
file(GLOB_RECURSE BOX2D_SRC Box2D/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# compile the static lib
add_library(box2d STATIC ${BOX2D_SRC})
