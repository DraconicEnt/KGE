/**
 *  @file FileBitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <physfs.h>

#include <support/CFileBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        CFileBitStream::CFileBitStream(const Support::String& fileName, const BITSTREAM_MODE& fileMode) : mFileHandle(nullptr)
        {
            if (fileMode == BITSTREAM_READ)
                mFileHandle = PHYSFS_openRead(fileName.c_str());
            else if (fileMode == BITSTREAM_WRITE)
                mFileHandle = PHYSFS_openWrite(fileName.c_str());
            else
                mFileHandle = PHYSFS_openAppend(fileName.c_str());

            //if (!mFileHandle)
            //    throw std::not_
        }

        CFileBitStream::~CFileBitStream(void)
        {
            if (mFileHandle)
                PHYSFS_close(mFileHandle);

            mFileHandle = nullptr;
        }
    } // EndNameSpace Support
} // End NameSpace Kiaro
