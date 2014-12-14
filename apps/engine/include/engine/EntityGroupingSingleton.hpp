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

#ifndef _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_

#include <set>  // std::set

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class EntityBase;
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

            // Private Methods
            private:
                //! Standard constructor
                EntityGroupingSingleton(void);
                //! Standard destructor
                ~EntityGroupingSingleton(void);

                bool addEntity(Kiaro::Game::Entities::EntityBase *entity);

            // Private Members
            private:
                std::set<Kiaro::Game::Entities::EntityBase *> mEntities;
                std::set<Kiaro::Game::Entities::EntityBase *> mNetworkedEntities;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
