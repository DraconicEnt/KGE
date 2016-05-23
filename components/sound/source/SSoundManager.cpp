/**
 *  @file SSoundManager.cpp
 */

#include <sound/SSoundManager.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Sound
    {
        static SSoundManager* mInstance = nullptr;

        SSoundManager* SSoundManager::getPointer(void)
        {
            if (!mInstance)
                mInstance = new SSoundManager();
            return mInstance;
        }

        void SSoundManager::destroy(void)
        {
            delete mInstance;
            mInstance = nullptr;
        }

        SSoundManager::SSoundManager(void)
        {
            // dsp, stream, geometry, total

            FMOD_RESULT result;
            if ((result = FMOD::System_Create(&mFMod)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to initialize FMod! Code: %u", result);
                return;
            }

            Common::U32 fmodVersion;
            if ((result = mFMod->getVersion(&fmodVersion)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to query FMod version! Code: %u", result);

                this->~SSoundManager();
                return;
            }

            if (fmodVersion != FMOD_VERSION)
            {
                CONSOLE_ERROR("FMod header version doesn't match our library's version!");

                this->~SSoundManager();
                return;
            }

            CONSOLE_INFO("Initialized F-Mod sound system.");
        }

        SSoundManager::~SSoundManager(void)
        {
            if (mFMod)
                mFMod->release();

            mFMod = nullptr;
        }

        CSoundSource* SSoundManager::getSoundSource(const Support::String& filename)
        {
            CSoundSource* sound = new CSoundSource(mFMod);

            return sound;
        }
    }
}
