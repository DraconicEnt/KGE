/**
 *  @file CPlayer.hpp
 *  @brief Include file declaring the CPlayer class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entity
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_CPLAYER_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_CPLAYER_HPP_

#include <support/common.hpp>

#include <game/entities/IRigidObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                /**
                 *  @brief A player is a controllable entity that represents a controllable bipedal character in the game world.
                 */
                class CPlayer : public IRigidObject
                {
                    // Public Members
                    public:
                        //! An event type for when a player is killed by something.
                        typedef EasyDelegate::DelegateSet<CPlayer*> OnPlayerKilledEvent;

                        //! Global on player killed responders.
                        static OnPlayerKilledEvent sOnPlayerKilledResponders;

                        //! Player specific on killed responders.
                        OnPlayerKilledEvent mOnPlayerKilledResponders;

                    // Public Methods
                    public:
                        CPlayer(void);

                        CPlayer(Support::CBitStream& in);

                        void packEverything(Support::CBitStream& out) const;

                        void unpack(Support::CBitStream& in);

                        size_t getRequiredMemory(void) const;

                        void update(const Common::F32 deltaTimeSeconds);

                        void registerEntity(void);
                };
            }
        } // End Namespace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_PLAYER_HPP_
