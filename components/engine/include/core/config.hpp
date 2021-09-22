/**
 *  @file config.hpp
 *  @brief The configuration file for the game engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#if !defined(_INCLUDE_KIARO_CONFIG_H_)
#define _INCLUDE_KIARO_CONFIG_H_

//! The maximum number of logger hooks we can have in the console.
#define MAXIMUM_LOGGER_HOOKS 32
//! The engine tickrate in milliseconds.
#define ENGINE_TICKRATE 32
//! How many command line arguments can be parsed by CCommandlineParser.
#define MAXIMUM_COMMANDLINE_ARGUMENTS 20
//! The maximum length of strings to be considered valid when read through CBitStream.
#define MAXIMUM_ARBITRARY_STRING_LENGTH 256

//! How long each worker thread should sleep per iteration.
#define WORKER_SLEEP_TIME_MS 64
#define WORKER_THREAD_POOL_SIZE 6

#ifndef CMAKE_CONFIG
#define MAXIMUM_DELTATIME
#endif

// If defined, the engine will use an arena allocation model for all entities in the engine. This offers a large speed boost at the
// expense of having an entity count high limit that increments on a per entity type basis only.
// #define ENGINE_ENTITY_ARENA_ALLOCATIONS

// If defined and ENGINE_ENTITY_ARENA_ALLOCATIONS is defined, the engine will use this value for arena size allocations instead of
// loading it from the configuration. This is useful for final distributions.
#define ENGINE_ENTITY_ARENA_ALLOCATION_SIZE 256

#endif
