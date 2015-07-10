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

#include <core/Logging.hpp>
#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>

#include <game/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        SGameWorld *sInstance = NULL;

        SGameWorld *SGameWorld::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SGameWorld;

            return sInstance;
        }

        void SGameWorld::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = NULL;
        }

        bool SGameWorld::addEntity(Entities::IEntity* entity)
        {
            if (mAvailableIDs.empty())
                return false;

            const Common::U32& identifier = mAvailableIDs.top();

            // Paranoia check
            if (identifier >= 4096)
                throw std::out_of_range("SGameworld: Got invalid entity identifier!");

            mEntities[identifier] = entity;

            const Common::U32 hintMask = entity->getHintMask();

            if (!(hintMask & Entities::NO_THINKING))
                mUpdatedEntities[identifier] = entity;

            if (!(hintMask & Entities::NO_UPDATING))
                mNetworkedEntities[identifier] = entity;

            // New sky?
            if (entity->getTypeMask() == Entities::ENTITY_SKY)
            {
                // FIXME (Robert MacGregor#9): Properly replace the sky entity
                if (mSky)
                {
                    Core::Logging::write(Core::Logging::MESSAGE_ERROR, "SGameWorld: Overwrote an old instance of the sky!");
                }

                mSky = (Entities::CSky*)entity;
            }

            entity->setNetID(identifier);
            mAvailableIDs.pop();
            return true;
        }

        bool SGameWorld::destroyEntitiy(const Common::U32& identifier)
        {
            if (mEntities[identifier])
            {
                delete mEntities[identifier];
                mEntities[identifier] = NULL;
                mUpdatedEntities[identifier] = NULL;
                mNetworkedEntities[identifier] = NULL;

                mAvailableIDs.push(identifier);
                return true;
            }

            return false;
        }

        const Entities::IEntity* const* SGameWorld::getEntities(void) const
        {
            return mEntities;
        }

/*
        const Support::Set<Game::Entities::IEntity* >& SGameWorld::getUpdatedEntities(void)
        {
            return mUpdatedEntities;
        }

        const Support::Set<Game::Entities::IEntity* >& SGameWorld::getNetworkedEntities(void)
        {
            return mNetworkedEntities;
        }
*/

        void SGameWorld::update(const Common::F32& deltaTimeSeconds)
        {
            for (Common::U32 iteration = 0; iteration < 4096; iteration++)
                if (mUpdatedEntities[iteration])
                    mUpdatedEntities[iteration]->update(deltaTimeSeconds);
        }

        const Entities::CSky* SGameWorld::getSky(void)
        {
            return mSky;
        }

        void SGameWorld::clear(void)
        {
            // Destroy any existing entities and reset the ID tracker
            mAvailableIDs = Support::Stack<Common::U32>();

            for (Common::U32 iteration = 0; iteration < 4096; iteration++)
            {
                mAvailableIDs.push(iteration);

                if (mEntities[iteration])
                    delete mEntities[iteration];

                mEntities[iteration] = NULL;
                mUpdatedEntities[iteration] = NULL;
                mNetworkedEntities[iteration] = NULL;
            }
        }

        SGameWorld::SGameWorld(void) : mSky(NULL)
        {
            // Initialize the pointer arrays to NULL
            memset(mEntities, 0x00, sizeof(Entities::IEntity*) * 4096);
            memset(mUpdatedEntities, 0x00, sizeof(Entities::IEntity*) * 4096);
            memset(mNetworkedEntities, 0x00, sizeof(Entities::IEntity*) * 4096);

            this->clear();
        }

        SGameWorld::~SGameWorld(void) { }
    } // End Namespace Engine
} // End Namespace Kiaro
