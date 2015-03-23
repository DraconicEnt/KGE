/**
 *  @file SEventManager.cpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/SEventManager.hpp>

namespace Kiaro
{
    namespace Engine
    {
        static SEventManager *sSingletonInstance = NULL;

        SEventManager *SEventManager::get(void)
        {
            if (!sSingletonInstance)
                sSingletonInstance = new SEventManager;

            return sSingletonInstance;
        }

        void SEventManager::destroy(void)
        {
            if (sSingletonInstance)
            {
                delete sSingletonInstance;
                sSingletonInstance = NULL;
            }
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

