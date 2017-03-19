/**
 *  @file CVoice.hpp
 *  @brief Include file declaring the CVoice class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SOUND_IVOICE_HPP_
#define _INCLUDE_SOUND_IVOICE_HPP_

#include <support/common.hpp>
#include <support/types.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief A class representing a voice for playing back a given sound.
         */
        class IVoice
        {
            // Public Methods
            public:
                virtual Support::Vector3DF getPosition(void) = 0;

                virtual Support::Vector3DF getVelocity(void) = 0;

                virtual void setPlaybackTimeMS(const Common::U32& ms) = 0;

                virtual void setPositionVelocity(const Support::Vector3DF& position, const Support::Vector3DF& velocity) = 0;
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
#endif // _INCLUDE_SOUND_CVOICE_HPP_
