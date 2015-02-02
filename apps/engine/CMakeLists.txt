# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Engine)

# Options specifically for the engine project
OPTION(BUILD_ASM "Build the engine with x86 assembly optimizations." OFF)

IF (BUILD_ASM)
	MESSAGE(WARNING "Building the engine with x86 assembly optimizations.")
	ADD_DEFINITIONS(-DENGINE_INLINE_ASM=1)
ENDIF (BUILD_ASM)

# The BUILD_UNITTESTS value is set by the upper project
IF (BUILD_UNITTESTS)
	ADD_DEFINITIONS(-DENGINE_TESTS=1)
ENDIF (BUILD_UNITTESTS)

# Configure the Project itself
file(GLOB_RECURSE ENGINE_SOURCES "source/*.cpp")
file(GLOB_RECURSE ENGINE_INCLUDES "include/*.hpp")

SET (ENGINE_BUILDLOCATION "../../bin/KGE")

IF (WIN32)
	SET (ENGINE_BUILDLOCATION "${KIARO_BUILDLOCATION}.exe")
	#SET (ENGINE_SOURCES "${KIARO_SOURCES}source/resource.rc")
ENDIF (WIN32)

# Hacky, should probably find a better method of accomplishing this link
LINK_DIRECTORIES("../lua/")
INCLUDE_DIRECTORIES ("include/" "../lua/include" ${Boost_INCLUDE_DIRS} ${PHYSFS_INCLUDE_DIR} ${IRRLICHT_INCLUDE_DIRS} ${ENet_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} ../${CEGUI_INCLUDE_DIR})
ADD_EXECUTABLE (${ENGINE_BUILDLOCATION} ${ENGINE_SOURCES} ${ENGINE_INCLUDES})
ADD_DEPENDENCIES(${ENGINE_BUILDLOCATION} Lua)
TARGET_LINK_LIBRARIES(${ENGINE_BUILDLOCATION} ${BULLET_LIBRARIES} ${PHYSFS_LIBRARY} ${ENet_LIBRARIES} ${IRRLICHT_LIBRARIES} ${Boost_LIBRARIES} ${GTEST_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_IrrlichtRenderer_LIBRARIES} "pthread" "lua.a") 
