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

#include <game/entities/Types.hpp>
#include <game/entities/EntityBase.hpp>

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
            mEntities.insert(mEntities.end(), entity);

            const Kiaro::Common::U32 hintMask = entity->getHintMask();

            if (!(hintMask & Kiaro::Game::Entities::ENTITYHINT_NONTHINKING))
                mUpdatedEntities.insert(mUpdatedEntities.end(), entity);

            if (!(hintMask & Kiaro::Game::Entities::ENTITYHINT_NONUPDATING))
                mNetworkedEntities.insert(mNetworkedEntities.end(), entity);

            if (entity->getTypeMask() & Kiaro::Game::Entities::ENTITY_SKY)
            {
                if (mSky)
                    std::cerr << "EntityGroupingSingleton: Overwrote an old instance of the Sky!" << std::endl;

                mSky = (Kiaro::Game::Entities::Sky*)entity;
            }

            return false;
        }

        const std::set<Kiaro::Game::Entities::EntityBase *> &EntityGroupingSingleton::getEntities(void)
        {
            return mEntities;
        }

        const std::set<Kiaro::Game::Entities::EntityBase *> &EntityGroupingSingleton::getUpdatedEntities(void)
        {
            return mUpdatedEntities;
        }

        const std::set<Kiaro::Game::Entities::EntityBase *> &EntityGroupingSingleton::getNetworkedEntities(void)
        {
            return mNetworkedEntities;
        }

        void EntityGroupingSingleton::update(const Kiaro::Common::F32 &deltaTimeSeconds)
        {
            for (std::set<Kiaro::Game::Entities::EntityBase *>::iterator it = mUpdatedEntities.begin(); it != mUpdatedEntities.end(); it++)
                (*it)->update(deltaTimeSeconds);
        }

        const Kiaro::Game::Entities::Sky *EntityGroupingSingleton::getSky(void)
        {
            return mSky;
        }

        EntityGroupingSingleton::EntityGroupingSingleton(void) : mSky(NULL) { }

        EntityGroupingSingleton::~EntityGroupingSingleton(void) { }
    } // End Namespace Engine
} // End Namespace Kiaro
