/**
 *  @file RigidProp.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_RIGIDPROP_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_RIGIDPROP_HPP_

#include "engine/Common.hpp"

#include <game/entities/RigidObjectBase.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class RigidProp : public Kiaro::Game::Entities::RigidObjectBase
            {
                // Public Methods
                public:
                    RigidProp(void);
                    ~RigidProp(void);

                    void packUpdate(Kiaro::Support::BitStream &out);
                    void unpackUpdate(Kiaro::Support::BitStream &in);
                    void packInitialization(Kiaro::Support::BitStream &out);
                    void unpackInitialization(Kiaro::Support::BitStream &in);
                    void instantiate(void);

                    void update(const Kiaro::Common::F32 &deltaTimeSeconds);
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_RIGIDPROP_HPP_
