/**
 *  @file SSoundManager.cpp
 */

#include <physfs.h>

#include <sound/SSoundManager.hpp>

#include <support/Console.hpp>

#include <fmod_errors.h>

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
            FMOD_RESULT result;
            if ((result = FMOD::System_Create(&mFMod)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to create FMod system! Reason: %s", FMOD_ErrorString(result));
                return;
            }

            Common::U32 fmodVersion;
            if ((result = mFMod->getVersion(&fmodVersion)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to query FMod version! Reason: %s", FMOD_ErrorString(result));

                this->~SSoundManager();
                return;
            }

            if (fmodVersion != FMOD_VERSION)
            {
                CONSOLE_ERROR("FMod header version doesn't match our library's version!");

                this->~SSoundManager();
                return;
            }

            // Init FMod
            if ((result = mFMod->init(32, FMOD_INIT_NORMAL, nullptr)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to initialize FMod! Reason: %s", FMOD_ErrorString(result));
                this->~SSoundManager();
                return;
            }

            if ((result = mFMod->setFileSystem(this->fmodOpen, this->fmodClose, this->fmodRead, this->fmodSeek, nullptr, nullptr, 0)) != FMOD_OK)
            {
                CONSOLE_ERRORF("Failed to set FMod file system! Reason: %s", FMOD_ErrorString(result));
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

        void SSoundManager::update(void)
        {
            mFMod->update();
        }

        CSoundSource* SSoundManager::getSoundSource(const Support::String& filename)
        {
            CSoundSource* sound = new CSoundSource(mFMod);

            return sound;
        }

        FMOD_RESULT SSoundManager::fmodOpen(const Common::C8* name, Common::U32* filesize, void** handle, void* userdata)
        {
            if (!PHYSFS_exists(name))
                return FMOD_ERR_FILE_NOTFOUND;

            PHYSFS_File* result = PHYSFS_openRead(name);

            if (!result)
                return FMOD_ERR_INTERNAL;

            *handle = result;
            *filesize = PHYSFS_fileLength(result);

            return FMOD_OK;
        }

        FMOD_RESULT SSoundManager::fmodClose(void* handle, void* userdata)
        {
            if (!PHYSFS_close(reinterpret_cast<PHYSFS_File*>(handle)))
                return FMOD_ERR_INTERNAL;

            return FMOD_OK;
        }

        FMOD_RESULT SSoundManager::fmodRead(void* handle, void* buffer, Common::U32 sizebytes, Common::U32* bytesread, void* userdata)
        {
            unsigned long physfsBytesRead = PHYSFS_read(reinterpret_cast<PHYSFS_File*>(handle), buffer, 1, sizebytes);

            if (physfsBytesRead == -1)
                return FMOD_ERR_INTERNAL;

            *bytesread = static_cast<Common::U32>(physfsBytesRead);
            return FMOD_OK;
        }

        FMOD_RESULT SSoundManager::fmodSeek(void* handle, Common::U32 pos, void *userdata)
        {
            if (!PHYSFS_seek(reinterpret_cast<PHYSFS_File*>(handle), pos))
                return FMOD_ERR_INTERNAL;

            return FMOD_OK;
        }
    }
}
