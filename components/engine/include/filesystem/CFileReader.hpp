/**
 *  @file CFileReader.hpp
 *  @brief Include file declaring the CFileReader class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
#define _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_

#include <physfs.h>

#include <support/support.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace FileSystem
    {
        /*
        class CFileReader : public irr::io::IReadFile
        {
            // Public Methods
            public:
                CFileReader(const Support::String& filename);

                ~CFileReader(void);

                irr::s32 read(void* buffer, irr::u32 sizeToRead);

                bool seek(long finalPos, bool relativeMovement = false);

                long getSize() const;

                long getPos() const;

                const irr::io::path& getFileName() const;

            // Private Members
            private:
                const Support::String mFileName;
                const irr::io::path mIrrlichtPath;

                PHYSFS_File* mFileHandle;
        };
        */
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
