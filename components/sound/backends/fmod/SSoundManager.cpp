/**
 *  @file SSoundManager.cpp
 *  @brief Source file implementing the SSoundManager singleton class FMod binding.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <physfs.h>
#include <fmod_errors.h>

#include <support/Console.hpp>

#include <sound/SSoundManager.hpp>

namespace Kiaro
{
    namespace Sound
    {
        static FMOD_RESULT F_CALLBACK fmodOpen(const Common::C8* name, Common::U32* filesize, void** handle, void* userdata)
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

        static FMOD_RESULT F_CALLBACK fmodClose(void* handle, void* userdata)
        {
            if (!PHYSFS_close(reinterpret_cast<PHYSFS_File*>(handle)))
                return FMOD_ERR_INTERNAL;

            return FMOD_OK;
        }

        static FMOD_RESULT F_CALLBACK fmodRead(void* handle, void* buffer, Common::U32 sizebytes, Common::U32* bytesread, void* userdata)
        {
            unsigned long physfsBytesRead = PHYSFS_read(reinterpret_cast<PHYSFS_File*>(handle), buffer, 1, sizebytes);

            if (physfsBytesRead == -1)
                return FMOD_ERR_INTERNAL;

            *bytesread = static_cast<Common::U32>(physfsBytesRead);
            return FMOD_OK;
        }

        static FMOD_RESULT F_CALLBACK fmodSeek(void* handle, Common::U32 pos, void* userdata)
        {
            if (!PHYSFS_seek(reinterpret_cast<PHYSFS_File*>(handle), pos))
                return FMOD_ERR_INTERNAL;

            return FMOD_OK;
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

            if ((result = mFMod->setFileSystem(fmodOpen, fmodClose, fmodRead, fmodSeek, nullptr, nullptr, 0)) != FMOD_OK)
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
            // Does the sound exist?
            auto it = mSoundRegistry.find(filename);

            if (it != mSoundRegistry.end())
                return (*it).second;

            // If PhysFS says we can't have it, return nullptr
            if (!PHYSFS_exists(filename.data()))
            {
                CONSOLE_ERRORF("Can't find sound file: %s", filename.data());
                return nullptr;
            }

            CSoundSource* sound = new CSoundSource(mFMod, filename.data());
            mSoundRegistry[filename] = sound;
            return sound;
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
