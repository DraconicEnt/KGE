# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring EasyLua Dep ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (EasyLua)

# Configure the Project itself
file(GLOB_RECURSE EASYLUA_INCLUDES "include/*.hpp")
file(GLOB_RECURSE EASYLUA_SOURCES "source/*.cpp")

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" ${LUA_INCLUDE_DIRECTORY})
ADD_LIBRARY (EasyLua "STATIC" ${EASYLUA_SOURCES} ${EASYLUA_INCLUDES})

GET_FILENAME_COMPONENT(EASYLUA_INCLUDE_DIRECTORY "include/" ABSOLUTE)
SET(EASYLUA_INCLUDE_DIRECTORY ${EASYLUA_INCLUDE_DIRECTORY} PARENT_SCOPE)