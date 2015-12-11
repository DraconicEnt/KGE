/**
 *  @file SGameWorld.cpp
 *  @brief Source file containing coding for the SGameWorld tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/UnorderedMap.hpp>

#include <game/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        TEST(SGameWorld, Listener)
        {
            SGameWorld* world = SGameWorld::getPointer();
            
            SGameWorld::destroy();
        }
    } // End Namespace Support
} // End namespace Kiaro

 
