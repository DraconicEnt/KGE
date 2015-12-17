/**
 *  @file SObjectRegistry.hpp
 */

#ifndef _INCLUDE_GAME_SOBJECTREGISTRY_HPP_
#define _INCLUDE_GAME_SOBJECTREGISTRY_HPP_

#include <support/Stack.hpp>
#include <support/Deque.hpp>
#include <support/UnorderedMap.hpp>
#include <support/String.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IEngineObject;
        
        class SObjectRegistry
        {
            friend class IEngineObject;
            
            // Private Members
            private:
                Support::Deque<IEngineObject*> mObjects;
                
                //! A stack of available object ID's arranged in the order we need them to be in when popping them.
                Support::Stack<Common::U32> mAvailableIDs;
                
                Support::UnorderedMap<size_t, IEngineObject*> mNameDictionary;
            
            // Protected Methods
            protected:
                Common::U32 getNextObjectID(void);
                
                void setNameEntry(IEngineObject* object, const Support::String& name);
                
            // Public Methods
            public:
                static SObjectRegistry* getPointer(void);
                static void destroy(void);
                
                bool addObject(IEngineObject* entity);
                bool removeObject(IEngineObject* entity);
                bool removeObject(const Common::U32& identifier);
                
                IEngineObject* getObject(const Common::U32& id);
                IEngineObject* getObject(const Support::String& name);
                
                void clear(void);
                
            // Private Methods
            private:
                ~SObjectRegistry(void);
        };
    }
}
#endif // _INCLUDE_GAME_SOBJECTREGISTRY_HPP_
