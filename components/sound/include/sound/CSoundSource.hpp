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

#ifndef _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
#define _INCLUDE_SOUND_CSOUNDSOURCE_HPP_

#include <fmod.hpp>
#include <support/String.hpp>
#include <support/UnorderedSet.hpp>

#include <sound/CVoice.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief A class representing a sound source associated with some given sound file.
         */
        class CSoundSource
        {
                // Private Members
            private:
                //! A pointer to the internal FMod sound.
                FMOD::Sound* mSound;
                //! A pointer to the internal FMod system.
                FMOD::System* mFMod;

                //! A list of voices associated with this source.
                Support::UnorderedSet<CVoice*> mVoices;

                // Public Methods
            public:
                /**
                 *  @brief Constructor accepting an FMod system.
                 *  @param system A pointer to the active FMod system.
                 */
                CSoundSource(FMOD::System* system, const Support::String& filename);

                /**
                 *  @brief Standard destructor.
                 */
                ~CSoundSource(void);

                /**
                 *  @brief Plays the sound source, creating a new voice.
                 *  @return A pointer to the created voice.
                 */
                CVoice* play(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
#endif // _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
