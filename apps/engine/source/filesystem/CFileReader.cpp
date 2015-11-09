/**
 *  @file FileReadObject.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <filesystem/CFileReader.hpp>

namespace Kiaro
{
    namespace FileSystem
    {
        FileReadObject::FileReadObject(const Support::String &filename) : mFileName(filename), mFileHandle(PHYSFS_openRead(filename.data()))
        {
            if (!mFileHandle)
                throw std::logic_error(PHYSFS_getLastError());
        }

        FileReadObject::~FileReadObject(void)
        {
            PHYSFS_close(mFileHandle);
        }

        irr::s32 FileReadObject::read(void* buffer, irr::u32 sizeToRead)
        {
            return PHYSFS_read(mFileHandle, buffer, sizeToRead, 1);
        }

        bool FileReadObject::seek(long finalPos, bool relativeMovement)
        {
            if (relativeMovement)
                finalPos += PHYSFS_tell(mFileHandle);

            return PHYSFS_seek(mFileHandle, finalPos);
        }

        long FileReadObject::getSize() const
        {
            return PHYSFS_fileLength(mFileHandle);
        }

        long FileReadObject::getPos() const
        {
            return PHYSFS_tell(mFileHandle);
        }

        const irr::io::path &FileReadObject::getFileName() const
        {
            return mFileName.c_str();
        }
    } // End Namespace Engine
} // End Namespace Kiaro
