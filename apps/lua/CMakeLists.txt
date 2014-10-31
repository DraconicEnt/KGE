# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Lua)

# Configure the Project itself
file(GLOB_RECURSE LUA_SOURCES "source/*.c")
file(GLOB_RECURSE LUA_SOURCES_CPP "source/*.cpp")
file(GLOB_RECURSE LUA_INCLUDES_HPP "include/*.hpp")
file(GLOB_RECURSE LUA_INCLUDES "include/*.hpp")

SET (LUA_BUILDLOCATION "lua")

IF (UNIX)
	ADD_DEFINITIONS(-DLUA_USE_MKSTEMP)
ENDIF(UNIX)

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" "../engine/include" ${Boost_INCLUDE_DIRS} ${PHYSFS_INCLUDE_DIR} ${IRRLICHT_INCLUDE_DIRS} ${ENet_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${CEGUI_INCLUDE_DIR})
ADD_LIBRARY (${LUA_BUILDLOCATION} "STATIC" ${LUA_SOURCES} ${LUA_SOURCES_CPP} ${LUA_INCLUDES} ${LUA_INCLUDES_HPP})
TARGET_LINK_LIBRARIES(${LUA_BUILDLOCATION} "m") 
