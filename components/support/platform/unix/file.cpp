/**
 *  @file file.cpp
 *  @brief Source file implementing platform Unix specific portions of the file systems in the engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdexcept>
#include <iostream>

#include <sys/mman.h>

#include <support/platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            void MemoryMappedFile::open(void)
            {
                this->close();

                if (File::exists(mFilePath))
                    mHandle = fopen(mFilePath.data(), "r+");
                else
                    mHandle = fopen(mFilePath.data(), "w+");

                if (!mHandle)
                    throw std::runtime_error("Unable to memory map file!");

                fseek(mHandle, 0, SEEK_END);
                const size_t fileLength = ftell(mHandle);
                fseek(mHandle, 0, SEEK_SET);
                mLength = mLength == 0 ? fileLength : mLength;
                const char nullBytes = 0x00;

                if (fileLength < mLength)
                {
                    fwrite(&nullBytes, 1, mLength - fileLength, mHandle);
                    fflush(mHandle);
                }

                int protection = 0;

                if (mAccessMode & ACCESS_MODE::WRITE)
                    protection |= PROT_WRITE;

                if (mAccessMode & ACCESS_MODE::READ)
                    protection |= PROT_READ;

                mAddress = mmap(nullptr, mLength, protection, MAP_SHARED, fileno(mHandle), mOffset);

                if (!mAddress)
                {
                    this->close();
                    throw std::runtime_error("Unable to obtain mapped memory from kernel!");
                }
            }

            void MemoryMappedFile::close(void)
            {
                if (mAddress)
                    munmap(mAddress, mLength);

                if (mHandle)
                    fclose(mHandle);

                mHandle = nullptr;
                mAddress = nullptr;
            }
        }
    }
}
