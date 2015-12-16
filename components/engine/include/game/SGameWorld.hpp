/**
 *  @file SGameWorld.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_
#define _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_

#include <support/Stack.hpp>
#include <support/Deque.hpp>
#include <support/UnorderedMap.hpp>
#include <support/String.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class IEntity;
            class CSky;
        } // End NameSpace Entities

        class SGameWorld
        {
            friend class Entities::IEntity;
            
            // Private Members
            private:
                Support::Deque<Entities::IEntity*> mEntities;

                Entities::CSky* mSky;

                //! A stack of available object ID's arranged in the order we need them to be in when popping them.
                Support::Stack<Common::U32> mAvailableIDs;
                
                Support::UnorderedMap<size_t, Entities::IEntity*> mNameDictionary;
                
            // Protected Methods
            protected:
                void setNameEntry(Entities::IEntity* entity, const Support::String& name);
                
                Common::U32 getNextEntityID(void);
                
            // Private Methods
            private:
                void repopulateIDStack(void);

            // Public Methods
            public:
                static SGameWorld* getPointer(void);
                static void destroy(void);

                bool addEntity(Entities::IEntity* entity);
                bool removeEntity(Entities::IEntity* entity);
                bool removeEntity(const Common::U32& identifier);
                
                Entities::IEntity* getEntity(const Support::String& name);

                void update(const Common::F32& deltaTimeSeconds);
                void clear(void);
                const Entities::CSky* getSky(void);

            // Private Methods
            private:
                //! Standard constructor
                SGameWorld(void);
                //! Standard destructor
                ~SGameWorld(void);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_SGAMEWORLD_HPP_
