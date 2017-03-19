/**
 *  @file CVoice.cpp
 *  @brief Source file implementing the CVoice class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <sound/CVoice.hpp>
#include <support/Console.hpp>

namespace Kiaro
{
    namespace Sound
    {
        void CVoice::setPlaybackTimeMS(const Common::U32& ms)
        {

        }

        void CVoice::setPositionVelocity(const Support::Vector3DF& position, const Support::Vector3DF& velocity)
        {

        }

        Support::Vector3DF CVoice::getPosition(void)
        {
            return Support::Vector3DF(0, 0, 0);
        }

        Support::Vector3DF CVoice::getVelocity(void)
        {
            return Support::Vector3DF(0, 0, 0);
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
