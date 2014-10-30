/**
 *  @file FileReadObject.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/FileReadObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        FileReadObject::FileReadObject(const std::string &filename) : mFileName(filename), mFileHandle(PHYSFS_openRead(filename.c_str()))
        {

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
