/**
 *  @file SGameWorld.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>

#include <game/IGameMode.hpp>
#include <game/SGameWorld.hpp>
#include <core/SObjectRegistry.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            void SGameWorld::update(const Common::F32 deltaTimeSeconds)
            {
                // FIXME: Implement bitmask checking for updated entities
                for (Support::UnorderedSet<Game::Entities::IEntity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    Game::Entities::IEntity* entity = *it;

                    if (entity && entity->mFlags & Entities::FLAG_UPDATING)
                    {
                        entity->update(deltaTimeSeconds);
                    }
                }
            }

            Entities::CSky* SGameWorld::getSky(void)
            {
                return mSky;
            }

            void SGameWorld::clear(void)
            {
                // Destroy any existing entities and reset the ID tracker
                for (Support::UnorderedSet<Game::Entities::IEntity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    delete *it;
                }

                mEntities.clear();
            }

            void SGameWorld::addEntity(Entities::IEntity* entity)
            {
                assert(entity);

                Support::UnorderedSet<Game::Entities::IEntity*>::iterator it = mEntities.find(entity);
                if (it == mEntities.end())
                {
                    mEntities.insert(mEntities.end(), entity);
                }

                Core::SObjectRegistry::getInstance()->addObject(entity);
            }

            void SGameWorld::removeEntity(Entities::IEntity* entity)
            {
                assert(entity);

                mEntities.erase(entity);
                Core::SObjectRegistry::getInstance()->removeObject(entity);
            }

            Entities::IEntity* SGameWorld::removeEntity(const Common::U32 id)
            {
                Entities::IEntity* erased = reinterpret_cast<Entities::IEntity*>(Core::SObjectRegistry::getInstance()->getObject(id));
                mEntities.erase(erased);

                return erased;
            }

            Entities::IEntity* SGameWorld::getEntity(const Common::U32 id) const
            {
                // FIXME: Type Check without using dynamic_cast
                Entities::IEntity* result = dynamic_cast<Entities::IEntity*>(Core::SObjectRegistry::getInstance()->getObject(id));
                return result;
            }

            Entities::IEntity* SGameWorld::getEntity(const Support::String& name) const
            {
                // FIXME: Type Check without using dynamic_cast
                Entities::IEntity* result = dynamic_cast<Entities::IEntity*>(Core::SObjectRegistry::getInstance()->getObject(name));
                return result;
            }

            void SGameWorld::packEverything(Support::CBitStream& out) const
            {
                for (Support::UnorderedSet<Game::Entities::IEntity*>::const_iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    (*it)->packEverything(out);
                }
            }

            void SGameWorld::unpack(Support::CBitStream& in)
            {
            }

            void SGameWorld::setGameMode(IGameMode* game)
            {
                if (mGameMode)
                {
                    mGameMode->tearDown();
                }

                mGameMode = game;

                if (mGameMode)
                {
                    mGameMode->setup();
                }
            }

            IGameMode* SGameWorld::getGameMode(void)
            {
                return mGameMode;
            }

            SGameWorld::SGameWorld(void) : mSky(nullptr), mGameMode(nullptr)
            {
            }

            SGameWorld::~SGameWorld(void) { }

            SGameWorld::iterator SGameWorld::begin(void)
            {
                return mEntities.begin();
            }

            SGameWorld::const_iterator SGameWorld::end(void)
            {
                return mEntities.end();
            }

            size_t SGameWorld::getRequiredMemory(void) const
            {
                return 0;
            }
        } // End Namespace Engine
    }
} // End Namespace Kiaro
