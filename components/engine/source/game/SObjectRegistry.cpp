/**
*  @file SObjectRegistry.cpp
*/

#include <game/SObjectRegistry.hpp>

#include <game/IEngineObject.hpp>

namespace Kiaro
{
    namespace Game
    {
        SObjectRegistry* sObjectRegistry = nullptr;

        SObjectRegistry* SObjectRegistry::getPointer(void)
        {
            if (!sObjectRegistry)
                sObjectRegistry = new SObjectRegistry();

            return sObjectRegistry;
        }

        void SObjectRegistry::destroy(void)
        {
            delete sObjectRegistry;
            sObjectRegistry = nullptr;
        }

        SObjectRegistry::~SObjectRegistry(void)
        {
            this->clear();
        }

        bool SObjectRegistry::addObject(IEngineObject* object)
        {
            assert(object);
            assert(object->mID <= mObjects.size());
            const Common::U32& identifier = object->mID;

            if (object->mID == mObjects.size())
                mObjects.insert(mObjects.end(), object);
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
                return false;

            mObjects[identifier] = nullptr;
            mAvailableIDs.push(identifier);
            return true;
        }

        void SObjectRegistry::setNameEntry(IEngineObject* object, const Support::String& name)
        {
            assert(!name.empty());

            if (!object)
            {
                mNameDictionary.erase(name);
                return;
            }

            mNameDictionary[name] = object;
        }

        bool SObjectRegistry::removeObject(IEngineObject* object)
        {
            return this->removeObject(object->mID);
        }

        Common::U32 SObjectRegistry::getNextObjectID(void)
        {
            Common::U32 result = mObjects.size();

            if (mAvailableIDs.size() != 0)
            {
                result = mAvailableIDs.top();
                mAvailableIDs.pop();
            }

            return result;
        }

        IEngineObject* SObjectRegistry::getObject(const Common::U32 id)
        {
            return id >= mObjects.size() ? nullptr : mObjects[id];
        }

        IEngineObject* SObjectRegistry::getObject(const Support::String& name)
        {
            auto it = mNameDictionary.find(name);
            return it == mNameDictionary.end() ? nullptr : (*it).second;
        }

        void SObjectRegistry::clear(void)
        {
            // Destroy each object in the registry and fill its spot with a null pointer
            for (IEngineObject*& object : mObjects)
            {
                if (object)
                    delete object;

                object = nullptr;
            }
        }
    }
}
