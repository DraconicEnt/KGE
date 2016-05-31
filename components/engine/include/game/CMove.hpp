/**
 *  @file CMove.hpp
 */

#ifndef _INCLUDE_GAME_CMOVE_HPP_
#define _INCLUDE_GAME_CMOVE_HPP_

#include <support/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CMove : public Support::ISerializable
        {
                // Public Members
            public:
                Common::F32 mX;
                Common::F32 mY;
                Common::F32 mZ;

                bool mTriggers[9];

                // Public Methods
            public:
                CMove(void);

                void reset(void);

                void packEverything(Support::CBitStream& out) const;

                void unpack(Support::CBitStream& in);

                size_t getRequiredMemory(void) const;
        };
    }
}
#endif // _INCLUDE_GAME_CMOVE_HPP_
