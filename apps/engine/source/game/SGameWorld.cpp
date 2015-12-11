/**
 *  @file SGameWorld.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <support/Console.hpp>
#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>

#include <game/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        SGameWorld* sInstance = nullptr;

        SGameWorld* SGameWorld::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SGameWorld;

            return sInstance;
        }

        void SGameWorld::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = nullptr;
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

          //  if (!(hintMask & Entities::NO_THINKING))
            //    mUpdatedEntities[identifier] = entity;

          //  if (!(hintMask & Entities::NO_UPDATING))
           //     mNetworkedEntities[identifier] = entity;

            // New sky?
            if (entity->getTypeMask() == Entities::ENTITY_SKY)
            {
                // FIXME (Robert MacGregor#9): Properly replace the sky entity
                if (mSky)
                    Support::Console::write(Support::Console::MESSAGE_ERROR, "SGameWorld: Overwrote an old instance of the sky!");

                mSky = (Entities::CSky*)entity;
            }

            entity->setNetID(identifier);
            mAvailableIDs.pop();
            return true;
        }
        
        void SGameWorld::repopulateIDStack(void)
        {
            mAvailableIDs = Support::Stack<Common::U32>();
            
            for (Common::S32 iteration = 4095; iteration >= 0; iteration--)
                mAvailableIDs.push(iteration);
        }
        
        Entities::IEntity* SGameWorld::getEntity(const Support::String& name)
        {
            const size_t nameHash = Support::getHashCode(name);
            
            auto it = mNameDictionary.find(nameHash);
            if (it == mNameDictionary.end())
                return nullptr;
            
            return (*it).second;
        }

        bool SGameWorld::destroyEntitiy(const Common::U32& identifier)
        {
            if (identifier >= mEntities.size())
                return false;
                
            delete mEntities[identifier];
            mEntities[identifier] = nullptr;
            
            mAvailableIDs.push(identifier);

            return false;
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
            // FIXME: Implement bitmask checking for updated entities
            for (auto it = mEntities.begin(); it != mEntities.end(); it++)
            {
                Entities::IEntity* entity = *it;
                
                if (entity)
                    entity->update(deltaTimeSeconds);
            }
        }

        const Entities::CSky* SGameWorld::getSky(void)
        {
            return mSky;
        }

        void SGameWorld::clear(void)
        {
            // Destroy any existing entities and reset the ID tracker
            mAvailableIDs = Support::Stack<Common::U32>();
            mNameDictionary.clear();
            
            for (auto it = mEntities.begin(); it != mEntities.end(); it++)
                delete *it;
                
            this->repopulateIDStack();
                
            mEntities.clear();
        }

        SGameWorld::SGameWorld(void) : mSky(nullptr)
        {
            this->repopulateIDStack();
        }

        SGameWorld::~SGameWorld(void) { }
    } // End Namespace Engine
} // End Namespace Kiaro
