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

#ifndef _INCLUDE_SOUND_CVOICE_HPP_
#define _INCLUDE_SOUND_CVOICE_HPP_

#include <fmod.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief A class representing a voice for playing back a given sound.
         */
        class CVoice
        {
                // Private Members
            private:
                //! Pointer to the FMod channel.
                FMOD::Channel* mChannel;

                // Public Methods
            public:
                /**
                 *  @brief Constructor accepting an FMod channel.
                 *  @param channel A pointer to the FMod channel to use.
                 */
                CVoice(FMOD::Channel* channel);

                //! Standard destructor.
                ~CVoice(void);

                void setPlaybackTimeMS(const Common::U32& ms);

                void setPositionVelocity(const Common::Vector3DF& position, const Common::Vector3DF& velocity);

                Common::Vector3DF getPosition(void);
                Common::Vector3DF getVelocity(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
#endif // _INCLUDE_SOUND_CVOICE_HPP_
