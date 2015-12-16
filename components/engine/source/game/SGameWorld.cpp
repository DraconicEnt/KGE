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
                sInstance = new SGameWorld();

            return sInstance;
        }

        void SGameWorld::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = nullptr;
        }
        
        void SGameWorld::setNameEntry(Entities::IEntity* entity, const Support::String& name)
        {
            assert(!name.empty());
            assert(entity);
            
            auto it = mNameDictionary.find(Support::getHashCode(name));
            if (it != mNameDictionary.end())
                mNameDictionary.erase(it);
                
            mNameDictionary[Support::getHashCode(name)] = entity;
        }
        
        Common::U32 SGameWorld::getNextEntityID(void)
        {
            Common::U32 result = mAvailableIDs.top();
            mAvailableIDs.pop();
            
            return result;
        }

        bool SGameWorld::addEntity(Entities::IEntity* entity)
        {
            if (mAvailableIDs.empty())
                return false;

            const Common::U32& identifier = entity->getID();

            // Paranoia check
            if (identifier >= 4096)
                throw std::out_of_range("SGameworld: Got invalid entity identifier!");

            mEntities[identifier] = entity;

            const Common::U32& flagMask = entity->mFlags;

            // New sky?
            if (entity->getTypeMask() & Entities::ENTITY_SKY)
            {
                // FIXME (Robert MacGregor#9): Properly replace the sky entity
                if (mSky)
                    Support::Console::write(Support::Console::MESSAGE_ERROR, "SGameWorld: Overwrote an old instance of the sky!");

                mSky = reinterpret_cast<Entities::CSky*>(entity);
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

        bool SGameWorld::removeEntity(const Common::U32& identifier)
        {
            if (identifier >= mEntities.size())
                return false;
                
            delete mEntities[identifier];
            mEntities[identifier] = nullptr;
            
            mAvailableIDs.push(identifier);

            return false;
        }
        
        bool SGameWorld::removeEntity(Entities::IEntity* entity)
        {
            return this->removeEntity(entity->getID());
        }

        void SGameWorld::update(const Common::F32& deltaTimeSeconds)
        {
            // FIXME: Implement bitmask checking for updated entities
            for (auto it = mEntities.begin(); it != mEntities.end(); it++)
            {
                Entities::IEntity* entity = *it;
                
                if (entity && entity->mFlags & Entities::FLAG_UPDATING)
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
