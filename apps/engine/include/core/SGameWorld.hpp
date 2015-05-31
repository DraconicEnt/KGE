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


#include <support/support.hpp>
#include <core/common.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class IEntity;
            class CSky;
        }
    }

    namespace Core
    {
        class SGameWorld
        {
            // Public Methods
            public:
                static SGameWorld *getPointer(void);
                static void destroy(void);

                bool addEntity(Game::Entities::IEntity *entity);

                const std::set<Game::Entities::IEntity *> &getEntities(void);
                const std::set<Game::Entities::IEntity *> &getUpdatedEntities(void);
                const std::set<Game::Entities::IEntity *> &getNetworkedEntities(void);

                void update(const Common::F32 &deltaTimeSeconds);
                const Game::Entities::CSky *getSky(void);

            // Private Methods
            private:
                //! Standard constructor
                SGameWorld(void);
                //! Standard destructor
                ~SGameWorld(void);

            // Private Members
            private:
                Support::Set<Game::Entities::IEntity *> mEntities;
                Support::Set<Game::Entities::IEntity *> mUpdatedEntities;
                Support::Set<Game::Entities::IEntity *> mNetworkedEntities;

                Kiaro::Game::Entities::CSky *mSky;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_
