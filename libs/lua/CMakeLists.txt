cmake_minimum_required(VERSION 2.8)
project(lua)

set( CMAKE_C_FLAGS "-DLUA_COMPAT_ALL=1 ${CMAKE_C_FLAGS}" )

set( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/pc CACHE PATH
        "Single Directory for all Libraries")

# all source files
file(GLOB_RECURSE LUA_SRC src/*.c)

# remove files which shouldn't be compiled with the lib
file(GLOB LUA_ALIEN_SRC 
	src/lua.c
	src/luac.c
)

# remove these files
list(REMOVE_ITEM LUA_SRC ${LUA_ALIEN_SRC})

# compile the static lib
add_library(lua STATIC ${LUA_SRC})
