/**
 *  @file EntityGroupingSingleton.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_

#include <set>  // std::set

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class EntityBase;
            class Sky;
        }
    }

    namespace Engine
    {
        class EntityGroupingSingleton
        {
            // Public Methods
            public:
                static EntityGroupingSingleton *getPointer(void);
                static void destroy(void);

                bool addEntity(Kiaro::Game::Entities::EntityBase *entity);

                const std::set<Kiaro::Game::Entities::EntityBase *> &getEntities(void);
                const std::set<Kiaro::Game::Entities::EntityBase *> &getUpdatedEntities(void);
                const std::set<Kiaro::Game::Entities::EntityBase *> &getNetworkedEntities(void);

                void update(const Kiaro::Common::F32 &deltaTimeSeconds);
                const Kiaro::Game::Entities::Sky *getSky(void);

            // Private Methods
            private:
                //! Standard constructor
                EntityGroupingSingleton(void);
                //! Standard destructor
                ~EntityGroupingSingleton(void);

            // Private Members
            private:
                std::set<Kiaro::Game::Entities::EntityBase *> mEntities;
                std::set<Kiaro::Game::Entities::EntityBase *> mUpdatedEntities;
                std::set<Kiaro::Game::Entities::EntityBase *> mNetworkedEntities;

                Kiaro::Game::Entities::Sky *mSky;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_
