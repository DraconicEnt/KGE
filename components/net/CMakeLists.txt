# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Net Component ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Net)

# Configure the Project itself
file(GLOB_RECURSE NET_INCLUDES "include/*.hpp")
file(GLOB_RECURSE NET_SOURCES "source/*.cpp")

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" "../engine/include" ${SUPPORT_INCLUDE_DIRECTORY} ${EASYDELEGATE_INCLUDE_DIRECTORY})
ADD_LIBRARY (Net "STATIC" ${NET_SOURCES} ${NET_INCLUDES})
ADD_DEPENDENCIES(Net Support)