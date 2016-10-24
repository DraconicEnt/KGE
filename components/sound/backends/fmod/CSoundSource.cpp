/**
 *  @file CSoundSource.cpp
 *  @brief Source file implementing the CSoundSource class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <fmod_errors.h>

#include <sound/CSoundSource.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Sound
    {
        CSoundSource::CSoundSource(FMOD::System* system, const Support::String& filename) : mSound(nullptr), mFMod(system)
        {
            FMOD_RESULT result;

            if ((result = system->createSound(filename.data(), FMOD_DEFAULT, nullptr, &mSound)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to create sound source! Reason: %s", FMOD_ErrorString(result));
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
                CONSOLE_ERRORF("Failed to play sound source! Reason: %s", FMOD_ErrorString(result));
                return nullptr;
            }

            CVoice* voice = new CVoice(channel);
            mVoices.insert(mVoices.end(), voice);
            return voice;
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
