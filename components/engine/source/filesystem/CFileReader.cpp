/**
 *  @file CFileReader.cpp
 *  @brief Source file implementing the CFileReader class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <filesystem/CFileReader.hpp>

namespace Kiaro
{
    namespace FileSystem
    {
            /*
        CFileReader::CFileReader(const Support::String& filename) : mFileName(filename), mFileHandle(PHYSFS_openRead(filename.data())), mIrrlichtPath(filename.data())
        {
            if (!mFileHandle)
                throw std::logic_error(PHYSFS_getLastError());
        }

        CFileReader::~CFileReader(void)
        {
            PHYSFS_close(mFileHandle);
        }

        irr::s32 CFileReader::read(void* buffer, irr::u32 sizeToRead)
        {
            return static_cast<irr::s32>(PHYSFS_read(mFileHandle, buffer, 1, sizeToRead));
        }

        bool CFileReader::seek(long finalPos, bool relativeMovement)
        {
            if (relativeMovement)
                finalPos += PHYSFS_tell(mFileHandle);

            return PHYSFS_seek(mFileHandle, static_cast<PHYSFS_uint64>(finalPos)) != 0;
        }

        long CFileReader::getSize() const
        {
            return PHYSFS_fileLength(mFileHandle);
        }

        long CFileReader::getPos() const
        {
            return PHYSFS_tell(mFileHandle);
        }

        const irr::io::path& CFileReader::getFileName() const
        {
            return mIrrlichtPath;
        }
        */
    } // End Namespace Engine
} // End Namespace Kiaro
