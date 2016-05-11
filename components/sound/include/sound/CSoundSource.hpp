/**
 *  @file SSoundSource.hpp
 */

#ifndef _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
#define _INCLUDE_SOUND_CSOUNDSOURCE_HPP_

#include <fmod.hpp>
#include <support/UnorderedSet.hpp>

#include <sound/CVoice.hpp>

namespace Kiaro
{
    namespace Sound
    {
        class CSoundSource
        {
            // Private Members
            private:
                FMOD::Sound* mSound;
                FMOD::System* mFMod;

                Support::UnorderedSet<CVoice*> mVoices;

            // Public Methods
            public:
                CSoundSource(FMOD::System* system);
                ~CSoundSource(void);

                CVoice* play(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
#endif // _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
