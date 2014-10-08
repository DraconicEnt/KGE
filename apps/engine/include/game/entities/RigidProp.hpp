/**
 *  @file RigidProp.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_RIGIDPROP_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_RIGIDPROP_HPP_

#include "engine/Common.hpp"

#include <game/entities/EntityBase.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class RigidProp : public Kiaro::Game::Entities::EntityBase
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
