/**
 *  @file SSoundManager.cpp
 *  @brief Source file implementing the SSoundManager singleton class FMod binding.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <physfs.h>

#include <support/Console.hpp>

#include <sound/SSoundManager.hpp>

namespace Kiaro
{
    namespace Sound
    {
        SSoundManager::SSoundManager(void)
        {
            CONSOLE_INFO("Initialized null sound system.");
        }

        SSoundManager::~SSoundManager(void)
        {

        }

        void SSoundManager::update(void)
        {

        }

        ISoundSource* SSoundManager::getSoundSource(const Support::String& filename)
        {
            return nullptr;
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
