/**
 */

#include <stdexcept>
#include <iostream>

#include <sys/mman.h>

#include <platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            MemoryMappedFile::MemoryMappedFile(const Support::String& filepath, const ACCESS_MODE& mode) : mAddress(nullptr),
            mLength(0), mFilePath(filepath), mAccessMode(mode), mOffset(0)
            {

            }

            void MemoryMappedFile::open(void)
            {
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

                char nullBytes[1] = { 0x00 };
                if (fileLength < mLength)
                {
                    fwrite(nullBytes, 1, mLength - fileLength, mHandle);
                    fclose(mHandle);
                    mHandle = fopen(mFilePath.data(), "r+");
                }

                int protection = 0;
                if (mAccessMode & ACCESS_MODE::WRITE)
                    protection |= PROT_WRITE;
                if (mAccessMode & ACCESS_MODE::READ)
                    protection |= PROT_READ;

                mAddress = mmap(nullptr, mLength, protection, MAP_SHARED, fileno(mHandle), mOffset);
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

            MemoryMappedFile::~MemoryMappedFile(void)
            {
                this->close();
            }
        }
    }
}
