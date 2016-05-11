/**
 *  @file SSoundSource.cpp
 */

#include <sound/CSoundSource.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Sound
    {
        CSoundSource::CSoundSource(FMOD::System* system) : mSound(nullptr), mFMod(system)
        {
            FMOD_RESULT result;
            if ((result = system->createSound("Bla", FMOD_DEFAULT, nullptr, &mSound)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to create sound source! Code: %u", result);
                return;
            }
        }

        CSoundSource::~CSoundSource(void)
        {
            if (mSound)
                mSound->release();

            mSound = nullptr;

            for (CVoice* voice: mVoices)
                delete voice;
        }

        CVoice* CSoundSource::play(void)
        {
            assert(mSound);

            FMOD_RESULT result;
            FMOD::Channel* channel = nullptr;
            if ((result = mFMod->playSound(mSound, 0, false, &channel)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to play sound source! Code: %u", result);
                return nullptr;
            }

            CVoice* voice = new CVoice(channel);
            mVoices.insert(mVoices.end(), voice);

            return voice;
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
