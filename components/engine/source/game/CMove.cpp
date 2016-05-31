/**
 *  @file CMove.cpp
 */

#include <game/CMove.hpp>

namespace Kiaro
{
    namespace Game
    {
        CMove::CMove(void)
        {
            this->reset();
        }

        void CMove::reset(void)
        {
            this->mX = 0;
            this->mY = 0;
            this->mZ = 0;

            for (Common::U32 iteration = 0; iteration < sizeof(mTriggers) / sizeof(bool); iteration++)
                mTriggers[iteration] = false;
        }

        void CMove::packEverything(Support::CBitStream& out) const
        {
            out << mX << mY << mZ;

            for (Common::U32 iteration = 0; iteration < sizeof(mTriggers) / sizeof(bool); iteration++)
                out << mTriggers[iteration];
        }

        void CMove::unpack(Support::CBitStream& in)
        {
            in >> mX >> mY >> mZ;

            for (Common::U32 iteration = 0; iteration < sizeof(mTriggers) / sizeof(bool); iteration++)
                in >> mTriggers[iteration];
        }

        size_t CMove::getRequiredMemory(void) const
        {
            return sizeof(Common::F32) * 3 * sizeof(bool) * sizeof(mTriggers);
        }
    }
}

