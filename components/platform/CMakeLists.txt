# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Platform Component ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Platform)

# Configure the Project itself
file(GLOB_RECURSE PLATFORM_INCLUDES "include/*.hpp")
file(GLOB_RECURSE PLATFORM_SOURCES "source/*.cpp")

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" "../engine/include" ${SUPPORT_INCLUDE_DIRECTORY})
ADD_LIBRARY (Platform "STATIC" ${PLATFORM_SOURCES} ${PLATFORM_INCLUDES})
ADD_DEPENDENCIES(Platform Support)