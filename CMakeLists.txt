# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(KiaroSDK)

# Preprocessor Definitions
SET(KIARO_VERSION_MAJOR 0)
SET(KIARO_VERSION_MINOR 0)
SET(KIARO_VERSION_REVISION 0)

ADD_DEFINITIONS(-DCMAKE_CONFIG=1)

# Optional Settings
OPTION(DEBUG "Build debug versions of all the software." OFF)
OPTION(BUILD_UNITTESTS "Build unit tests in all of the software. (It requires GTest)" ON)

# Options to build specific parts of the engine
OPTION(BUILD_ENGINE "Build the core game engine." ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Check our dependencies
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(Irrlicht REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(PhysFS REQUIRED)
FIND_PACKAGE(Boost "1.54.0" REQUIRED "system" "filesystem")
FIND_PACKAGE(CEGUI REQUIRED COMPONENTS IrrlichtRenderer)

# Might be useful for something later
#FIND_PACKAGE(SDL REQUIRED)

IF (BUILD_UNITTESTS)
    FIND_PACKAGE(GTest)
ELSE(BUILD_UNITTESTS)
    SET(GTEST_INCLUDE_DIRS "")
    SET(GTEST_LIBRARIES "")
ENDIF (BUILD_UNITTESTS)

# Raise a warning if we couldn't find GTest
IF (NOT GTEST_FOUND AND BUILD_UNITTESTS)
    SET(BUILD_UNITTESTS OFF)
    MESSAGE(WARNING "Did not find the Google Testing framework. Will build without unit tests.")
ENDIF (NOT GTEST_FOUND AND BUILD_UNITTESTS)

# TODO: Add /TP option to command line for Lua module to build when using MSVS

# Configure for GNU Compiler
IF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++11")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
    
    # To use std::thread and friends, we must pass -lpthread and -pthread to the compiler and Linker for GCC
    IF (NOT WIN32)
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -pthread")
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")
        SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lpthread -pthread" )
    ENDIF (NOT WIN32)

    IF (DEBUG)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    ENDIF (DEBUG)
ENDIF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)

# Handle our build options
IF (BUILD_ENGINE)
    ADD_SUBDIRECTORY("apps/lua")
    ADD_SUBDIRECTORY("apps/engine")
ENDIF (BUILD_ENGINE)
