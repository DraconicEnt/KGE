/**
 *  @file EntityGroupingSingleton.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/EntityGroupingSingleton.hpp>

namespace Kiaro
{
    namespace Engine
    {
        Kiaro::Engine::EntityGroupingSingleton *EntityGroupingSingleton_Instance = NULL;

        EntityGroupingSingleton *EntityGroupingSingleton::getPointer(void)
        {
            if (!EntityGroupingSingleton_Instance)
                EntityGroupingSingleton_Instance = new EntityGroupingSingleton;

            return EntityGroupingSingleton_Instance;
        }

        void EntityGroupingSingleton::destroy(void)
        {
            if (EntityGroupingSingleton_Instance)
                delete EntityGroupingSingleton_Instance;

            EntityGroupingSingleton_Instance = NULL;
        }

        bool EntityGroupingSingleton::addEntity(Kiaro::Game::Entities::EntityBase *entity)
        {
            return false;
        }

        EntityGroupingSingleton::EntityGroupingSingleton(void)
        {

        }

        EntityGroupingSingleton::~EntityGroupingSingleton(void)
        {

        }
    } // End Namespace Engine
} // End Namespace Kiaro
