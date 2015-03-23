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

#include <set>  // std::set

#include <engine/Common.hpp>

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

    namespace Engine
    {
        class SGameWorld
        {
            // Public Methods
            public:
                static SGameWorld *getPointer(void);
                static void destroy(void);

                bool addEntity(Kiaro::Game::Entities::IEntity *entity);

                const std::set<Kiaro::Game::Entities::IEntity *> &getEntities(void);
                const std::set<Kiaro::Game::Entities::IEntity *> &getUpdatedEntities(void);
                const std::set<Kiaro::Game::Entities::IEntity *> &getNetworkedEntities(void);

                void update(const Kiaro::Common::F32 &deltaTimeSeconds);
                const Kiaro::Game::Entities::CSky *getSky(void);

            // Private Methods
            private:
                //! Standard constructor
                SGameWorld(void);
                //! Standard destructor
                ~SGameWorld(void);

            // Private Members
            private:
                std::set<Kiaro::Game::Entities::IEntity *> mEntities;
                std::set<Kiaro::Game::Entities::IEntity *> mUpdatedEntities;
                std::set<Kiaro::Game::Entities::IEntity *> mNetworkedEntities;

                Kiaro::Game::Entities::CSky *mSky;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_ENTITYGROUPINGSINGLETON_HPP_
