/**
 *  @file Config.h
 *  @brief The configuration file for the game engine.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#if !defined(_INCLUDE_KIARO_CONFIG_H_)
    #define _INCLUDE_KIARO_CONFIG_H_

    // Configuration values that shouldn't need changing
    #define MAXIMUM_LOGGER_HOOKS 32
    #define ENGINE_TICKRATE 32
    #define MAXIMUM_COMMANDLINE_ARGUMENTS 20
    #define MAXIMUM_ARBITRARY_STRING_LENGTH 256

    #ifndef CMAKE_CONFIG
        #define MAXIMUM_DELTATIME
        #define ENGINE_TESTS 1
    #endif
#endif
