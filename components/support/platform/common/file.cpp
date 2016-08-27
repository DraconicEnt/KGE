/**
 *  @file file.cpp
 *  @brief Source file implementing platform independent portions of the file systems in the engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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

