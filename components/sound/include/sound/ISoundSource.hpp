/**
 *  @file CSoundSource.hpp
 *  @brief Include file declaring the CSoundSource class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SOUND_ISOUNDSOURCE_HPP_
#define _INCLUDE_SOUND_ISOUNDSOURCE_HPP_

#include <support/String.hpp>
#include <support/UnorderedSet.hpp>

#include <sound/IVoice.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief A class representing a sound source associated with some given sound file.
         */
        class ISoundSource
        {
            // Public Methods
            public:
                /**
                 *  @brief Plays the sound source, creating a new voice.
                 *  @return A pointer to the created voice.
                 */
                virtual IVoice* play(void) = 0;
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
#endif // _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
