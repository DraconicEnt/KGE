/**
 *  @file CGameWorld.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <game/IEntity.hpp>

#include <game/IGameMode.hpp>
#include <game/CGameWorld.hpp>
#include <core/SObjectRegistry.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            void CGameWorld::update(const Common::F32 deltaTimeSeconds)
            {
                // FIXME: Implement bitmask checking for updated entities
                for (Support::UnorderedSet<Game::IEntity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    Game::IEntity* entity = *it;

                    if (entity && entity->mFlags & Game::FLAG_UPDATING)
                    {
                        entity->update(deltaTimeSeconds);
                    }
                }
            }

            void CGameWorld::clear(void)
            {
                // Destroy any existing entities and reset the ID tracker
                for (Support::UnorderedSet<Game::IEntity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    delete *it;
                }

                mEntities.clear();
            }

            void CGameWorld::addEntity(IEntity* entity)
            {
                assert(entity);

                Support::UnorderedSet<Game::IEntity*>::iterator it = mEntities.find(entity);
                if (it == mEntities.end())
                {
                    mEntities.insert(mEntities.end(), entity);
                }

                Core::SObjectRegistry::getInstance()->addObject(entity);
            }

            void CGameWorld::removeEntity(IEntity* entity)
            {
                assert(entity);

                mEntities.erase(entity);
                Core::SObjectRegistry::getInstance()->removeObject(entity);
            }

            IEntity* CGameWorld::removeEntity(const Common::U32 id)
            {
                IEntity* erased = reinterpret_cast<IEntity*>(Core::SObjectRegistry::getInstance()->getObject(id));
                mEntities.erase(erased);

                return erased;
            }

            IEntity* CGameWorld::getEntity(const Common::U32 id) const
            {
                // FIXME: Type Check without using dynamic_cast
                IEntity* result = dynamic_cast<IEntity*>(Core::SObjectRegistry::getInstance()->getObject(id));
                return result;
            }

            IEntity* CGameWorld::getEntity(const Support::String& name) const
            {
                // FIXME: Type Check without using dynamic_cast
                IEntity* result = dynamic_cast<IEntity*>(Core::SObjectRegistry::getInstance()->getObject(name));
                return result;
            }

            void CGameWorld::packEverything(Support::CBitStream& out) const
            {
                for (Support::UnorderedSet<Game::IEntity*>::const_iterator it = mEntities.begin(); it != mEntities.end(); it++)
                {
                    (*it)->packEverything(out);
                }
            }

            void CGameWorld::unpack(Support::CBitStream& in)
            {
            }

            void CGameWorld::setGameMode(IGameMode* game)
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

            IGameMode* CGameWorld::getGameMode(void)
            {
                return mGameMode;
            }

            CGameWorld::CGameWorld(void) : mGameMode(nullptr)
            {
            }

            CGameWorld::~CGameWorld(void) { }

            CGameWorld::iterator CGameWorld::begin(void)
            {
                return mEntities.begin();
            }

            CGameWorld::const_iterator CGameWorld::end(void)
            {
                return mEntities.end();
            }

            size_t CGameWorld::getRequiredMemory(void) const
            {
                return 0;
            }
        } // End Namespace Engine
    }
} // End Namespace Kiaro
