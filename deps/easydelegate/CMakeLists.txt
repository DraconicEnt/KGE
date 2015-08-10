# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

MESSAGE(STATUS "Configuring EasyDelegate Dep ...")

# Configure the Project itself
FILE(GLOB_RECURSE EASYDELEGATE_INCLUDES "include/*.hpp")
GET_FILENAME_COMPONENT(EASYDELEGATE_INCLUDE_DIRECTORY "include/" ABSOLUTE)

SET(EASYDELEGATE_INCLUDES ${EASYDELEGATE_INCLUDES} PARENT_SCOPE)
SET(EASYDELEGATE_INCLUDE_DIRECTORY ${EASYDELEGATE_INCLUDE_DIRECTORY} PARENT_SCOPE)
