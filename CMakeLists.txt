cmake_minimum_required(VERSION 2.8)
project(x2d)

# set some project-wide flags
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-overflow" )

# we require boost with some linked libraries
find_package(Boost 1.51.0 COMPONENTS random system signals)

set( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/pc CACHE PATH
        "Single Directory for all Libraries")

# all source files
file(GLOB_RECURSE X2D_SRC x2d/src/*.cpp)

# find files not used in android build
file(GLOB X2D_ALIEN_SRC 
	x2d/src/sound/audio_queue.cpp
	x2d/src/sound/openal_fx.cpp
)

# remove files that are not needed for android
list(REMOVE_ITEM X2D_SRC ${X2D_ALIEN_SRC})

# add android glue
list(APPEND X2D_SRC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

if(X2D_DEBUG)
	# print them
	foreach(item ${X2D_SRC})
		message("Source file to compile: ${item}")
	endforeach(item)
endif()

# find all source directories
file(GLOB_RECURSE X2D_SRC_DIRS_ALL x2d/src/*.cpp x2d/src/*.h)
set(X2D_SRC_DIRS "")

# list x2d dirs to use as include paths
foreach(src_file ${X2D_SRC_DIRS_ALL})
	get_filename_component(comp ${src_file} PATH)
	list(APPEND X2D_SRC_DIRS ${comp})
endforeach(src_file)

# remove all duplicated entries to keep stuff tidy
list(REMOVE_DUPLICATES X2D_SRC_DIRS)

if(X2D_DEBUG)
        # print them
        foreach(item ${X2D_SRC_DIRS})
                message("Source dir to use for search paths: ${item}")
        endforeach(item)
endif()

# add some header-only libs
list(APPEND X2D_SRC_DIRS libs/glm libs/glm/gtc libs/rapidxml)

# compile libraries required by x2d
add_subdirectory(libs/lua)
add_subdirectory(libs/luabind)
add_subdirectory(libs/box2d)

# set includes
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${X2D_SRC_DIRS})
include_directories(libs/box2d libs/lua/src libs/luabind)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive" )

# compile x2d itself
add_library(x2d STATIC ${X2D_SRC})

set(LIBRARY_DEPS lua luabind box2d GLESv2 log)

# link final lib
target_link_libraries(x2d ${LIBRARY_DEPS} ${Boost_LIBRARIES})
