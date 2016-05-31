# - Try to find Allegro5
# Once done this will define
#
#  ALLEGRO_FOUND - system has Allegro
#  ALLEGRO_INCLUDE_DIRS - the Allegro include directory
#  ALLEGRO_LIBRARIES - Link these to use Allegro
#  ALLEGRO_DEFINITIONS - Compiler switches required for using Allegro
#
#  Copyright (c) 2008 Olof Naessen <olof.naessen@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
  # in cache already
  set(ALLEGRO_FOUND TRUE)
else (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
  find_path(ALLEGRO_INCLUDE_DIR
    NAMES
      allegro5.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    PATH_SUFFIXES
      allegro5
  )

  find_library(ALLEG_LIBRARY
    NAMES
      allegro
    PATHS
      /usr/lib/x86_64-linux-gnu/
      /usr/lib/i386-linux-gnu/
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  find_library(ALLEG_PHYSFS_LIBRARY
    NAMES
      allegro_physfs
    PATHS
      /usr/lib/x86_64-linux-gnu/
      /usr/lib/i386-linux-gnu/
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  set(ALLEGRO_INCLUDE_DIRS ${ALLEGRO_INCLUDE_DIR})
  set(ALLEGRO_LIBRARIES ${ALLEG_LIBRARY} ${ALLEG_PHYSFS_LIBRARY})

  if (ALLEGRO_INCLUDE_DIRS AND ALLEG_PHYSFS_LIBRARY AND ALLEG_LIBRARY)
     set(ALLEGRO_FOUND TRUE)
  endif (ALLEGRO_INCLUDE_DIRS AND ALLEG_PHYSFS_LIBRARY AND ALLEG_LIBRARY)

  if (ALLEGRO_FOUND)
    # Does our include dir desiginate allegro5?
    # We determine this by checking if we can see the base.h relative to this dir
    IF (EXISTS ${ALLEGRO_INCLUDE_DIR}/base.h )
    	GET_FILENAME_COMPONENT(ALLEGRO_INCLUDE_DIR ${ALLEGRO_INCLUDE_DIR} DIRECTORY)

	  set(ALLEGRO_INCLUDE_DIRS
	    ${ALLEGRO_INCLUDE_DIR}
    )
    ENDIF (EXISTS ${ALLEGRO_INCLUDE_DIR}/base.h )

    # Add a check For 5.1.12
    FILE(READ ${ALLEGRO_INCLUDE_DIR}/allegro5/base.h BASE_CONTENTS)

    IF (NOT DEFINED BASE_CONTENTS)
   	 MESSAGE(FATAL_ERROR "Could not open allegro5/base.h!")
    ENDIF (NOT DEFINED BASE_CONTENTS)

    # Look for the sub & wip versions
    STRING(REGEX MATCH "ALLEGRO_SUB_VERSION +[0-9]+" ALLEGRO_SUB_VERSION ${BASE_CONTENTS})
    STRING(REPLACE "ALLEGRO_SUB_VERSION" "" ALLEGRO_SUB_VERSION ${ALLEGRO_SUB_VERSION})
    STRING(STRIP ${ALLEGRO_SUB_VERSION} ALLEGRO_SUB_VERSION)

    STRING(REGEX MATCH "ALLEGRO_WIP_VERSION +[0-9]+" ALLEGRO_WIP_VERSION ${BASE_CONTENTS})
    STRING(REPLACE "ALLEGRO_WIP_VERSION" "" ALLEGRO_WIP_VERSION ${ALLEGRO_WIP_VERSION})
    STRING(STRIP ${ALLEGRO_WIP_VERSION} ALLEGRO_WIP_VERSION)

    # Build the version string and check it
    SET(ALLEGRO_VERSION  5.${ALLEGRO_SUB_VERSION}.${ALLEGRO_WIP_VERSION})
    IF (ALLEGRO_VERSION VERSION_EQUAL "5.1.12" OR ALLEGRO_VERSION VERSION_GREATER "5.1.12")
        SET(FOUND_MATCH TRUE)
    ENDIF (ALLEGRO_VERSION VERSION_EQUAL "5.1.12" OR ALLEGRO_VERSION VERSION_GREATER "5.1.12")

    IF (NOT FOUND_MATCH)
        MESSAGE(FATAL_ERROR "Located a Allegro library, but it is not 5.1.12 or newer!")
    ENDIF (NOT FOUND_MATCH)

    if (NOT Allegro_FIND_QUIETLY)
      message(STATUS "Found Allegro Core: ${ALLEG_LIBRARY}")
      message(STATUS "Found Allegro PhysFS: ${ALLEG_PHYSFS_LIBRARY}")
    endif (NOT Allegro_FIND_QUIETLY)
  else (ALLEGRO_FOUND)
    if (Allegro_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Allegro")
    endif (Allegro_FIND_REQUIRED)
  endif (ALLEGRO_FOUND)

  # show the ALLEGRO_INCLUDE_DIRS and ALLEGRO_LIBRARIES variables only in the advanced view
  mark_as_advanced(ALLEGRO_INCLUDE_DIRS ALLEGRO_LIBRARIES)

endif (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
