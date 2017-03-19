/**
 *  @file CCommandLineParser.cpp
 */

#include <gtest/gtest.h>

#include <sound/SSoundManager.hpp>
#include <sound/CSoundSource.hpp>

namespace Kiaro
{
    namespace Support
    {
        static void initializePhysicsFS(void)
        {
        }

        TEST(SSoundManager, SoundRegistry)
        {
            Sound::SSoundManager* manager = Sound::SSoundManager::getInstance();
            Sound::ISoundSource* sound = manager->getSoundSource("testfile.wav");

            EXPECT_TRUE(sound != nullptr);
            EXPECT_EQ(manager->getSoundSource("testfile.wav"), sound);

            Sound::SSoundManager::destroy();
        }

        TEST(SSoundManager, NoFile)
        {
            Sound::SSoundManager* manager = Sound::SSoundManager::getInstance();

            EXPECT_TRUE(manager->getSoundSource("nosuchfile.wav") != nullptr);
            Sound::SSoundManager::destroy();
        }
    } // End Namespace Support
} // End namespace Kiaro
