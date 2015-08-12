# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Phys Component ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Phys)

# Configure the Project itself
file(GLOB_RECURSE PHYS_INCLUDES "include/*.hpp")
file(GLOB_RECURSE PHYS_SOURCES "source/*.cpp")

# FIXME: Include dirs?
INCLUDE_DIRECTORIES ("include/" "../engine/include" ${BULLET_INCLUDE_DIRS} ${SUPPORT_INCLUDE_DIRECTORY} ${EASYDELEGATE_INCLUDE_DIRECTORY})
ADD_LIBRARY (Phys "STATIC" ${PHYS_SOURCES} ${PHYS_INCLUDES})
TARGET_LINK_LIBRARIES(Phys ${ENGINE_LINKLIBRARIES})
ADD_DEPENDENCIES(Phys Support)