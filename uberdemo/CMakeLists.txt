set(X2D_ROOT_RELATIVE ..)

cmake_minimum_required(VERSION 2.8)
project(x2d_game)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )

# we require boost
find_package(Boost 1.51.0)

# add x2d. specify the relative path above.
# TODO: rewrite as FindX2d.cmake and use find_package
add_subdirectory(${X2D_ROOT_RELATIVE} libx2d)

set( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/pc CACHE PATH
        "Single Directory for all Libraries")

# all source files
file(GLOB_RECURSE SRC src/*.cpp)

# add some header-only libs
list(APPEND X2D_SRC_DIRS 
	${X2D_ROOT_RELATIVE}/libs/glm 
	${X2D_ROOT_RELATIVE}/libs/glm/gtc 
	${X2D_ROOT_RELATIVE}/libs/rapidxml
)

# set includes
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${X2D_SRC_DIRS})
include_directories(
	${X2D_ROOT_RELATIVE}/libs/box2d 
	${X2D_ROOT_RELATIVE}/libs/lua/src 
	${X2D_ROOT_RELATIVE}/libs/luabind
)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

# find all x2d source directories
file(GLOB_RECURSE X2D_SRC_DIRS_ALL ${X2D_ROOT_RELATIVE}/x2d/src/*.cpp ${X2D_ROOT_RELATIVE}/x2d/src/*.h)
set(X2D_SRC_DIRS "")

# list x2d dirs to use as include paths
foreach(src_file ${X2D_SRC_DIRS_ALL})
        get_filename_component(comp ${src_file} PATH)
        list(APPEND X2D_SRC_DIRS ${comp})
endforeach(src_file)

# remove all duplicated entries to keep stuff tidy
list(REMOVE_DUPLICATES X2D_SRC_DIRS)
include_directories(${X2D_SRC_DIRS})

# compile
add_library(x2d_game SHARED ${SRC})

set(LIBRARY_DEPS x2d lua luabind box2d android log EGL GLESv1_CM)

# link final lib
target_link_libraries(x2d_game ${LIBRARY_DEPS} ${Boost_LIBRARIES})
