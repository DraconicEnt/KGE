# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Net Component ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Net)

# Configure the Project itself
FILE(GLOB_RECURSE NET_INCLUDES "include/*.hpp")
FILE(GLOB_RECURSE NET_SOURCES "source/*.cpp")

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" "../engine/include" ${SUPPORT_INCLUDE_DIRECTORY} ${EASYDELEGATE_INCLUDE_DIRECTORY})
ADD_LIBRARY (Net "STATIC" ${NET_SOURCES} ${NET_INCLUDES})
ADD_DEPENDENCIES(Net Support)

GET_FILENAME_COMPONENT(NET_INCLUDE_DIRECTORY "include" ABSOLUTE)
GET_FILENAME_COMPONENT(NET_LINK_FILES "./libNet.a" ABSOLUTE)
SET(NET_LINK_FILES ${NET_LINK_FILES} PARENT_SCOPE)
SET(NET_INCLUDE_DIRECTORY ${NET_INCLUDE_DIRECTORY} PARENT_SCOPE)