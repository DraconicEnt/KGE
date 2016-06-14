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
            Sound::SSoundManager* manager = Sound::SSoundManager::getPointer();
            Sound::CSoundSource* sound = manager->getSoundSource("testfile.wav");

            EXPECT_NE(nullptr, sound);
            EXPECT_EQ(manager->getSoundSource("testfile.wav"), sound);

            Sound::SSoundManager::destroy();
        }

        TEST(SSoundManager, NoFile)
        {
            Sound::SSoundManager* manager = Sound::SSoundManager::getPointer();

            EXPECT_EQ(nullptr, manager->getSoundSource("nosuchfile.wav"));
            Sound::SSoundManager::destroy();
        }
    } // End Namespace Support
} // End namespace Kiaro
