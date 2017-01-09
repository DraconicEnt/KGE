/**
 *  @file CRigidObject.hpp
 *  @brief Include file declaring the CRigidObject class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_

#include "support/common.hpp"

#include <game/entities/IRigidObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                class CRigidObject : public Game::Entities::IRigidObject
                {
                        // Public Methods
                    public:
                        CRigidObject(void);
                        ~CRigidObject(void);

                        void packUpdate(Support::CBitStream& out);
                        void unpackUpdate(Support::CBitStream& in);
                        void packInitialization(Support::CBitStream& out);
                        void unpackInitialization(Support::CBitStream& in);
                        void instantiate(void);

                        void update(const Common::F32 deltaTimeSeconds);
                };
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_
