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
        CVoice::CVoice(FMOD::Channel* channel)
        {
        }

        CVoice::~CVoice(void)
        {
        }

        void CVoice::setPlaybackTimeMS(const Common::U32& ms)
        {
            assert(mChannel);
            FMOD_RESULT result;

            if ((result = mChannel->setPosition(ms, FMOD_TIMEUNIT_MS)) != FMOD_OK)
                CONSOLE_ERRORF("Failed to set voice playback time! Code: %u", result);
        }

        void CVoice::setPositionVelocity(const Support::Vector3DF& position, const Support::Vector3DF& velocity)
        {
            assert(mChannel);
            FMOD_VECTOR fmodPosition = { position.x(), position.y(), position.z() };
            FMOD_VECTOR fmodVelocity = { velocity.x(), velocity.y(), velocity.z() };
            FMOD_RESULT result;

            if ((result = mChannel->set3DAttributes(&fmodPosition, &fmodVelocity)) != FMOD_OK)
                CONSOLE_ERRORF("Failed to set voice position & velocity! Code: %u", result);
        }

        Support::Vector3DF CVoice::getPosition(void)
        {
            assert(mChannel);
            FMOD_VECTOR position;
            FMOD_VECTOR velocity;
            FMOD_RESULT result;

            if ((result = mChannel->get3DAttributes(&position, &velocity)) != FMOD_OK)
                CONSOLE_ERRORF("Failed to get voice position! Code: %u", result);

            return Support::Vector3DF(position.x, position.y, position.z);
        }

        Support::Vector3DF CVoice::getVelocity(void)
        {
            assert(mChannel);
            FMOD_VECTOR position;
            FMOD_VECTOR velocity;
            FMOD_RESULT result;

            if ((result = mChannel->get3DAttributes(&position, &velocity)) != FMOD_OK)
                CONSOLE_ERRORF("Failed to get voice velocity! Code: %u", result);

            return Support::Vector3DF(velocity.x, velocity.y, velocity.z);
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
