# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring Phys Component ...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Phys)

# Configure the Project itself
FILE(GLOB_RECURSE PHYS_INCLUDES "include/*.hpp")
FILE(GLOB_RECURSE PHYS_SOURCES "source/*.cpp")

INCLUDE_DIRECTORIES ("include/" ${BULLET_INCLUDE_DIRS} ${SUPPORT_INCLUDE_DIRECTORY} ${EASYDELEGATE_INCLUDE_DIRECTORY})
ADD_LIBRARY (Phys "STATIC" ${PHYS_SOURCES} ${PHYS_INCLUDES})
# TARGET_LINK_LIBRARIES(Phys ${ENGINE_LINKLIBRARIES})
ADD_DEPENDENCIES(Phys Support)

GET_FILENAME_COMPONENT(PHYS_LINK_FILES "./libPhys.a" ABSOLUTE)
SET(PHYS_LINK_FILES ${PHYS_LINK_FILES} PARENT_SCOPE)