/**
 *  @file SObjectRegistry.cpp
 *  @brief Source file implementing the SObjectRegistry singleton class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <core/SObjectRegistry.hpp>
#include <game/IEngineObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            SObjectRegistry::SObjectRegistry(void)
            {

            }

            SObjectRegistry::~SObjectRegistry(void)
            {
                this->clear();
            }

            bool SObjectRegistry::addObject(Game::IEngineObject* object)
            {
                assert(object);
                assert(object->mID <= mObjects.size());

                const Common::U32& identifier = object->mID;

                if (object->mID == mObjects.size())
                {
                    mObjects.insert(mObjects.end(), object);
                }
                else
                {
                    // Don't overwrite existing objects
                    assert(!mObjects[identifier] || object == mObjects[identifier]);
                    mObjects[identifier] = object;
                }

                return true;
            }

            bool SObjectRegistry::removeObject(const Common::U32 identifier)
            {
                if (identifier >= mObjects.size())
                {
                    return false;
                }

                mObjects[identifier] = nullptr;
                mAvailableIDs.push(identifier);
                return true;
            }

            void SObjectRegistry::setNameEntry(Game::IEngineObject* object, const Support::String& name)
            {
                CONSOLE_ASSERT(!name.empty(), "Name parameter was empty.");

                if (!object)
                {
                    mNameDictionary.erase(name);
                    return;
                }

                mNameDictionary[name] = object;
            }

            bool SObjectRegistry::removeObject(Game::IEngineObject* object)
            {
                return this->removeObject(object->mID);
            }

            Common::U64 SObjectRegistry::getNextObjectID(void)
            {
                Common::U64 result = mObjects.size();

                if (mAvailableIDs.size() != 0)
                {
                    result = mAvailableIDs.top();
                    mAvailableIDs.pop();
                }

                return result;
            }

            Game::IEngineObject* SObjectRegistry::getObject(const Common::U32 id)
            {
                return id >= mObjects.size() ? nullptr : mObjects[id];
            }

            Game::IEngineObject* SObjectRegistry::getObject(const Support::String& name)
            {
                auto it = mNameDictionary.find(name);
                return it == mNameDictionary.end() ? nullptr : (*it).second;
            }

            void SObjectRegistry::clear(void)
            {
                // Destroy each object in the registry and fill its spot with a null pointer
                for (Game::IEngineObject*& object : mObjects)
                {
                    if (object)
                    {
                        delete object;
                    }

                    object = nullptr;
                }
            }
        }
    }
}
