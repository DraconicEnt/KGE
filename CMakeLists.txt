# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2016 Draconic Entity

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(KiaroSDK)

# Accept an app name
SET(KGE_APP_NAME "KGE" CACHE STRING "Application name.")
IF (NOT DEFINED KGE_APP_NAME)
    MESSAGE(FATAL_ERROR "KGE_APP_NAME must be defined. (Perhaps try passing -DKGE_APP_NAME=<Your App Name>?)")
ENDIF (NOT DEFINED KGE_APP_NAME)

# We can optionally define the template, but we will default to basic.
SET(KGE_TEMPLATE_NAME "basic" CACHE STRING "Template name.")
IF (NOT DEFINED KGE_TEMPLATE_NAME)
    MESSAGE(WARNING "KGE_TEMPLATE_NAME not defined. Defaulting to 'basic'.")

    SET(KGE_TEMPLATE_NAME "basic")
ELSE (NOT DEFINED KGE_TEMPLATE_NAME)
    STRING(TOLOWER ${KGE_TEMPLATE_NAME} KGE_TEMPLATE_NAME)
ENDIF (NOT DEFINED KGE_TEMPLATE_NAME)

# We can optionally define an author, but the default is "Your Name / Company Name"
SET(KGE_TEMPLATE_NAME "No name" CACHE STRING "Author name.")
IF (NOT DEFINED KGE_AUTHOR_NAME)
    MESSAGE(WARNING "KGE_AUTHOR_NAME not defined. Defaulting to 'Your Name / Company Name'.")

    SET(KGE_AUTHOR_NAME "Your Name / Company Name")
ENDIF(NOT DEFINED KGE_AUTHOR_NAME)
SET(KGE_APP_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${KGE_APP_NAME})

# Preprocessor Definitions
SET(KIARO_VERSION_MAJOR 0)
SET(KIARO_VERSION_MINOR 0)
SET(KIARO_VERSION_REVISION 0)

ADD_DEFINITIONS(-DCMAKE_CONFIG=1)

# Optional Settings
OPTION(BUILD_UNITTESTS "Build unit tests in all of the software. (It requires GTest)" OFF)
OPTION(ENGINE_UB "Use Unity builds when compiling the engine." OFF)
OPTION(USE_BOOST_REGEX "Use boost::regex instead of std::regex." OFF)

# Options to build specific parts of the engine
OPTION(BUILD_ENGINE "Build the core game engine." ON)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Check our dependencies
FIND_PACKAGE(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgGA)
FIND_PACKAGE(PhysFS REQUIRED)
FIND_PACKAGE(MyGUI REQUIRED)
FIND_PACKAGE(Allegro REQUIRED COMPONENTS Main PhysFS NativeDialog)
FIND_PACKAGE(FMod REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(PythonInterp REQUIRED)
FIND_PACKAGE(CURL REQUIRED)

# GCC 4.9 implements std::regex, some versions prior implement a buggy version, so force a dep on boost::regex if we we're using < 4.9
IF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    IF (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.9")
        IF (NOT USE_BOOST_REGEX)
            MESSAGE(WARNING "Instructed to NOT use boost::regex (USE_BOOST_REGEX=OFF). However, you are using a GCC version that's older than 4.9 which has std::regex bugs. Overridding.")
        ENDIF (NOT USE_BOOST_REGEX)

        SET(USE_BOOST_REGEX ON)
    ENDIF(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.9")
ENDIF(CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)

IF (USE_BOOST_REGEX)
    FIND_PACKAGE(Boost REQUIRED COMPONENTS regex)
    ADD_DEFINITIONS(-DENGINE_USE_BOOST_REGEX=1)
ENDIF(USE_BOOST_REGEX)

IF (BUILD_UNITTESTS)
    FIND_PACKAGE(GTest)
ELSE(BUILD_UNITTESTS)
    SET(GTEST_INCLUDE_DIRS "")
    SET(GTEST_LIBRARIES "")
ENDIF (BUILD_UNITTESTS)

# Raise a warning if we couldn't find GTest and we wanted to use it
IF (NOT GTEST_FOUND AND BUILD_UNITTESTS)
    SET(BUILD_UNITTESTS OFF)
    MESSAGE(WARNING "Did not find the Google Testing framework. Will build without unit tests.")
ENDIF (NOT GTEST_FOUND AND BUILD_UNITTESTS)

# Configure for GNU Compiler
IF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    # We require C++11 to build anything here
    SET(CMAKE_CXX_FLAGS "-std=c++11")

    # To use std::thread and friends, we must pass -lpthread and -pthread to the compiler and Linker for GCC
    IF (NOT WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -pthread")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread -pthread")
    ENDIF (NOT WIN32)
ENDIF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)

IF (ENGINE_UB)
    ADD_DEFINITIONS(-DENGINE_UB=1)
ENDIF(ENGINE_UB)

# Make sure our actual projects are aware of the system type
IF (UNIX)
    ADD_DEFINITIONS(-DENGINE_UNIX=1)
ELSEIF(WIN32)
    ADD_DEFINITIONS(-DENGINE_WIN=1)
ELSE(WIN32)
    ADD_DEFINITIONS(-DENGINE_APPLE=1)
ENDIF(UNIX)

# Handle our build options
IF (BUILD_ENGINE)
    ADD_SUBDIRECTORY("deps")
    ADD_SUBDIRECTORY("components")
    ADD_SUBDIRECTORY("apps")
ENDIF (BUILD_ENGINE)
