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
            if (sObjectRegistry)
                delete sObjectRegistry;
                
            sObjectRegistry = nullptr;
        }
        
        bool SObjectRegistry::addObject(IEngineObject* object)
        {
            const Common::U32& identifier = object->mID;
            
            assert(object->mID <= mObjects.size());
            
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
        
        bool SObjectRegistry::removeObject(const Common::U32& identifier)
        {
            if (identifier >= mObjects.size())
                return false;
            
            mObjects[identifier] = nullptr;
            mAvailableIDs.push(identifier);

            return false;
        }
        
        void SObjectRegistry::setNameEntry(IEngineObject* object, const Support::String& name)
        {
            assert(!name.empty());
            assert(object);
            
            auto it = mNameDictionary.find(Support::getHashCode(name));
            if (it != mNameDictionary.end())
                mNameDictionary.erase(it);
                
            mNameDictionary[Support::getHashCode(name)] = object;
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
        
        IEngineObject* SObjectRegistry::getObject(const Common::U32& id)
        {
            if (id >= mObjects.size())
                return nullptr;
            
            return mObjects[id];
        }
        
        IEngineObject* SObjectRegistry::getObject(const Support::String& name)
        {
            const size_t nameHash = Support::getHashCode(name);
            
            auto it = mNameDictionary.find(nameHash);
            if (it == mNameDictionary.end())
                return nullptr;
            
            return (*it).second;      
        }
        
        void SObjectRegistry::clear(void)
        {
            for (auto it = mObjects.begin(); it != mObjects.end(); it++)
            {
                IEngineObject* object = *it;
                
                if (object)
                    delete object;
            }
        }
        
        SObjectRegistry::~SObjectRegistry(void)
        {
            this->clear();
        }
    }
}
