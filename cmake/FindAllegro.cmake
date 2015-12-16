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

  set(ALLEGRO_INCLUDE_DIRS
    ${ALLEGRO_INCLUDE_DIR}
  )
  set(ALLEGRO_LIBRARIES
    ${ALLEG_LIBRARY}
)

  if (ALLEGRO_INCLUDE_DIRS AND ALLEGRO_LIBRARIES)
     set(ALLEGRO_FOUND TRUE)
  endif (ALLEGRO_INCLUDE_DIRS AND ALLEGRO_LIBRARIES)

  if (ALLEGRO_FOUND)
    # Add a check For 5.1.12
    GET_FILENAME_COMPONENT(ALLEGRO_LIBRARY_DIR ${ALLEG_LIBRARY} DIRECTORY)
    FILE(GLOB_RECURSE ALLEGRO_MATCHES "${ALLEGRO_LIBRARY_DIR}/liballegro.so.5.*")
    #MESSAGE(WARNING ${ALLEGRO_MATCHES})

    SET(FOUND_MATCH FALSE)
    FOREACH (FILE ${ALLEGRO_MATCHES})
        GET_FILENAME_COMPONENT(CURRENT_FILE ${FILE} NAME)
        STRING(REGEX MATCH "liballegro.so.5.[1-9].[1-9]" FILE_MATCHES ${CURRENT_FILE})

        # We found a string with that format, good. We just need to check the last
        # two version components for 5.1.12 or newer
        IF (FILE_MATCHES)
            STRING(REPLACE "liballegro.so." "" CURRENT_VERSION ${CURRENT_FILE})

            IF (${CURRENT_VERSION} VERSION_EQUAL "5.1.12" OR ${CURRENT_VERSION} VERSION_GREATER "5.1.12")
                SET(FOUND_MATCH TRUE)
                BREAK()
            ENDIF (${CURRENT_VERSION} VERSION_EQUAL "5.1.12" OR ${CURRENT_VERSION} VERSION_GREATER "5.1.12")
        ENDIF (FILE_MATCHES)
    ENDFOREACH(FILE ${ALLEGRO_MATCHES})

    IF (NOT FOUND_MATCH)
        MESSAGE(FATAL_ERROR "Located a Allegro library, but it is not 5.1.12 or newer!")
    ENDIF (NOT FOUND_MATCH)

    IF (NOT EXISTS ${ALLEGRO_LIBRARIES}.5.1)
        MESSAGE(FATAL_ERROR "Unable to locate Allegro 5.1.12 or newer.")
    ENDIF (NOT EXISTS ${ALLEGRO_LIBRARIES}.5.1)

    if (NOT Allegro_FIND_QUIETLY)
      message(STATUS "Found Allegro: ${ALLEGRO_LIBRARIES}")
    endif (NOT Allegro_FIND_QUIETLY)
  else (ALLEGRO_FOUND)
    if (Allegro_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Allegro")
    endif (Allegro_FIND_REQUIRED)
  endif (ALLEGRO_FOUND)

  # show the ALLEGRO_INCLUDE_DIRS and ALLEGRO_LIBRARIES variables only in the advanced view
  mark_as_advanced(ALLEGRO_INCLUDE_DIRS ALLEGRO_LIBRARIES)

endif (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
