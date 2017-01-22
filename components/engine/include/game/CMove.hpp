/**
 *  @file CMove.hpp
 *  @brief Include file declaring the CMove class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_CMOVE_HPP_
#define _INCLUDE_GAME_CMOVE_HPP_

#include <support/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            /**
             *  @brief Class representing a move state for any given frame to be used to calculate the controlled object state for the
             *  next frame.
             *  @details The CMove is your basic block of networked input. All objects that can be influenced by external input store a
             *  CMove instance or read a CMove from their control source every frame to calculate the next object state for the next
             *  frame.
             */
            class CMove : public Support::ISerializable
            {
                // Public Methods
                public:
                    //! Parameter-less constructor.
                    CMove(void);

                    //! Resets the move to its default state which is no input at all.
                    void reset(void);

                    /**
                     *  @brief Serializes the CMove into a CBitStream.
                     *  @param out A reference to a CBitStream to pack the CMove into.
                     */
                    void packEverything(Support::CBitStream& out) const;

                    /**
                     *  @brief Deserializes a CMove from a CBitStream, writing the new move data into this instance.
                     *  @param in A reference to a CBitStream to unpack from.
                     */
                    void unpack(Support::CBitStream& in);

                    /**
                     *  @brief Helper method used to determine how much space is required in a CBitStream to serialize this CMove into.
                     *  @return The number of bytes required for serialization.
                     */
                    size_t getRequiredMemory(void) const;

                // Public Members
                public:
                    //! A float representing movement along the X axis. It is up to the controlled object what to make of this.
                    Common::F32 mX;

                    //! A float representing movement along the Y axis. It is up to the controlled object what to make of this.
                    Common::F32 mY;

                    //! A float representing movement along the Z axis. It is up to the controlled object what to make of this.
                    Common::F32 mZ;

                    //! An array of miscellaneous trigger states. It is up to the controlled object what to make of these.
                    bool mTriggers[9];

            };
        }
    }
}
#endif // _INCLUDE_GAME_CMOVE_HPP_
