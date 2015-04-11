/**
 *  @file CRigidObject.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_

#include "engine/common.hpp"

#include <game/entities/IRigidObject.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class CRigidObject : public Kiaro::Game::Entities::IRigidObject
            {
                // Public Methods
                public:
                    CRigidObject(void);
                    ~CRigidObject(void);

                    void packUpdate(Kiaro::Support::CBitStream &out);
                    void unpackUpdate(Kiaro::Support::CBitStream &in);
                    void packInitialization(Kiaro::Support::CBitStream &out);
                    void unpackInitialization(Kiaro::Support::CBitStream &in);
                    void instantiate(void);

                    void update(const Kiaro::Common::F32 &deltaTimeSeconds);
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_CRIGIDOBJECT_HPP_
