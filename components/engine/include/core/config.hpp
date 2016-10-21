/**
 *  @file Config.h
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

// Configuration values that shouldn't need changing
#define MAXIMUM_LOGGER_HOOKS 32
#define ENGINE_TICKRATE 32
#define MAXIMUM_COMMANDLINE_ARGUMENTS 20
#define MAXIMUM_ARBITRARY_STRING_LENGTH 256

#define WORKER_SLEEP_TIME_MS 64
#define WORKER_THREAD_POOL_SIZE 6

#ifndef CMAKE_CONFIG
#define MAXIMUM_DELTATIME
#define ENGINE_TESTS 1
#endif

// If defined, the engine will use an arena allocation model for all entities in the engine. This offers a large speed boost at the
// expense of having an entity count high limit that increments on a per entity type basis only.
#define ENGINE_ENTITY_ARENA_ALLOCATIONS

// If defined and ENGINE_ENTITY_ARENA_ALLOCATIONS is defined, the engine will use this value for arena size allocations instead of
// loading it fromt he configuration. This is useful for final distributions.
#define ENGINE_ENTITY_ARENA_ALLOCATION_SIZE 256

#endif
