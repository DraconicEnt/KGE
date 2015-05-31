/**
 *  @file SGameWorld.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>

#include <core/SGameWorld.hpp>

namespace Kiaro
{
    namespace Core
    {
        Kiaro::Core::SGameWorld *EntityGroupingSingleton_Instance = NULL;

        SGameWorld *SGameWorld::getPointer(void)
        {
            if (!EntityGroupingSingleton_Instance)
                EntityGroupingSingleton_Instance = new SGameWorld;

            return EntityGroupingSingleton_Instance;
        }

        void SGameWorld::destroy(void)
        {
            if (EntityGroupingSingleton_Instance)
                delete EntityGroupingSingleton_Instance;

            EntityGroupingSingleton_Instance = NULL;
        }

        bool SGameWorld::addEntity(Game::Entities::IEntity *entity)
        {
            mEntities.insert(mEntities.end(), entity);

            const Common::U32 hintMask = entity->getHintMask();

            if (!(hintMask & Game::Entities::ENTITYHINT_NONTHINKING))
                mUpdatedEntities.insert(mUpdatedEntities.end(), entity);

            if (!(hintMask & Game::Entities::ENTITYHINT_NONUPDATING))
                mNetworkedEntities.insert(mNetworkedEntities.end(), entity);

            if (entity->getTypeMask() & Game::Entities::ENTITY_SKY)
            {
                if (mSky)
                    std::cerr << "SGameWorld: Overwrote an old instance of the Sky!" << std::endl;

                mSky = (Game::Entities::CSky*)entity;
            }

            return false;
        }

        const Support::Set<Game::Entities::IEntity *> &SGameWorld::getEntities(void)
        {
            return mEntities;
        }

        const Support::Set<Game::Entities::IEntity *> &SGameWorld::getUpdatedEntities(void)
        {
            return mUpdatedEntities;
        }

        const Support::Set<Game::Entities::IEntity *> &SGameWorld::getNetworkedEntities(void)
        {
            return mNetworkedEntities;
        }

        void SGameWorld::update(const Common::F32 &deltaTimeSeconds)
        {
            for (auto it = mUpdatedEntities.begin(); it != mUpdatedEntities.end(); it++)
                (*it)->update(deltaTimeSeconds);
        }

        const Game::Entities::CSky *SGameWorld::getSky(void)
        {
            return mSky;
        }

        SGameWorld::SGameWorld(void) : mSky(NULL) { }

        SGameWorld::~SGameWorld(void) { }
    } // End Namespace Engine
} // End Namespace Kiaro
