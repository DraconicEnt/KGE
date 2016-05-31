/**
 *  @file SEventManager.cpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <game/SEventManager.hpp>

namespace Kiaro
{
    namespace Core
    {
        static SEventManager* sInstance = nullptr;

        SEventManager* SEventManager::get(void)
        {
            if (!sInstance)
                sInstance = new SEventManager;

            return sInstance;
        }

        void SEventManager::destroy(void)
        {
            delete sInstance;
            sInstance = nullptr;
        }

        void SEventManager::clearEvents(void)
        {
        }

        SEventManager::SEventManager(void)
        {
        }

        SEventManager::~SEventManager(void)
        {
        }
    } // End NameSpace Support
} // End nameSpace Kiaro

