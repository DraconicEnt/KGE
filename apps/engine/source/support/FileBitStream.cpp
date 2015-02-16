/**
 *  @file FileBitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <physfs.h>

#include <support/FileBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        FileBitStream::FileBitStream(const Kiaro::Common::String &fileName, const BITSTREAM_MODE &fileMode) : mFileHandle(NULL)
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

        FileBitStream::~FileBitStream(void)
        {
            if (mFileHandle)
                PHYSFS_close(mFileHandle);

            mFileHandle = NULL;
        }
    } // EndNameSpace Support
} // End NameSpace Kiaro
