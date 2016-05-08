/**
 *  @file support.cpp
 *  @brief Include file including all the engine support implementations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/CBitStream.hpp>
#include <support/CCommandLineParser.hpp>
#include <support/CFileBitStream.hpp>
#include <support/CMapDivision.hpp>
#include <support/FEndian.hpp>
#include <support/FTime.hpp>

#include <support/Regex.hpp>

#include <support/Math.hpp>
#include <support/Tuple.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>
#include <support/Map.hpp>

#include <support/types.hpp>

// Ensure that we're building against an SDL-enabled Irrlicht
#if !defined(_IRR_COMPILE_WITH_SDL_DEVICE_) || defined(NO_IRR_COMPILE_WITH_SDL_DEVICE_)
//    #error The engine must be built against an SDL device enabled Irrlicht build.
#endif

