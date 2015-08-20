# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Lua Component ...")


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Lua)

# Configure the Project itself
file(GLOB_RECURSE LUA_INCLUDES_HPP "include/*.hpp")
file(GLOB_RECURSE LUA_INCLUDES "include/*.hpp")
    
IF(NOT ENGINE_UB)
    file(GLOB_RECURSE LUA_SOURCES "source/*.c")
ELSE (NOT ENGINE_UB)
    SET(LUA_SOURCES "source/UB_main.c")
ENDIF (NOT ENGINE_UB)

IF (UNIX)
	ADD_DEFINITIONS(-DLUA_USE_MKSTEMP)
ENDIF(UNIX)


INCLUDE_DIRECTORIES ("include/" "../engine/include" ${IRRLICHT_INCLUDE_DIRS} ${PHYSFS_INCLUDE_DIR})
ADD_LIBRARY (Lua "STATIC" ${LUA_SOURCES} ${LUA_SOURCES_CPP} ${LUA_INCLUDES} ${LUA_INCLUDES_HPP})
TARGET_LINK_LIBRARIES(Lua "m") 

# Ensure that the parent can see our includes
GET_FILENAME_COMPONENT(LUA_INCLUDE_DIRECTORY "include/" ABSOLUTE)
GET_FILENAME_COMPONENT(LUA_LINK_LIBRARIES "./libLua.a" ABSOLUTE)

SET(LUA_INCLUDE_DIRECTORY ${LUA_INCLUDE_DIRECTORY} PARENT_SCOPE)
SET(LUA_LINK_LIBRARIES ${LUA_LINK_LIBRARIES} PARENT_SCOPE)
