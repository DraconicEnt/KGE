/**
 */

#include <stdexcept>
#include <iostream>

#include <support/platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            MemoryMappedFile::MemoryMappedFile(const Support::String& filepath, const ACCESS_MODE& mode) : mAddress(nullptr),
            mLength(0), mFilePath(filepath), mAccessMode(mode), mOffset(0), mHandle(nullptr), mData(nullptr)
            {

            }

            MemoryMappedFile::~MemoryMappedFile(void)
            {
                this->close();
            }
        }
    }
}

