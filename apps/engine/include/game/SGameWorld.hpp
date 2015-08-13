/**
 *  @file SGameWorld.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_
#define _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_

#include <support/Stack.hpp>
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
            // Private Members
            private:
                //! A set of all active entities in the game server.
                Entities::IEntity* mEntities[4096];

                //! A set of all entities that receive a logic tick.
                Entities::IEntity* mUpdatedEntities[4096];
                //!
                Entities::IEntity* mNetworkedEntities[4096];

                Entities::CSky* mSky;

                Support::Stack<Common::U32> mAvailableIDs;

            // Public Methods
            public:
                static SGameWorld* getPointer(void);
                static void destroy(void);

                bool addEntity(Entities::IEntity* entity);
                bool destroyEntitiy(const Common::U32& identifier);

                const Entities::IEntity* const* getEntities(void) const;

                //const Support::Set<Entities::IEntity*>& getEntities(void);
               // const Support::Set<Entities::IEntity*>& getUpdatedEntities(void);
               // const Support::Set<Entities::IEntity*>& getNetworkedEntities(void);

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
#endif // _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_
