/**
 *  @file FileReadObject.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
#define _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_

#include <irrlicht.h>

#include <physfs.h>

#include <support/support.hpp>
#include <core/common.hpp>

namespace Kiaro
{
    namespace FileSystem
    {
        class FileReadObject : public irr::io::IReadFile
        {
            // Public Methods
            public:
                FileReadObject(const Support::String &filename);

                ~FileReadObject(void);

                irr::s32 read(void* buffer, irr::u32 sizeToRead);

                bool seek(long finalPos, bool relativeMovement = false);

                long getSize() const;

                long getPos() const;

                const irr::io::path& getFileName() const;

            // Private Members
            private:
                const Support::String mFileName;
                PHYSFS_File *mFileHandle;
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
