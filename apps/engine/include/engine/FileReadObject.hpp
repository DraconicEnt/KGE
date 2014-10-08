/**
 *  @file FileReadObject.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
#define _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_

#include <engine/Common.hpp>
#include <irrlicht.h>

#include <physfs.h>

namespace Kiaro
{
    namespace Engine
    {
        class FileReadObject : public irr::io::IReadFile
        {
            // Public Methods
            public:
                FileReadObject(const std::string &filename);

                ~FileReadObject(void);

                irr::s32 read(void* buffer, irr::u32 sizeToRead);

                bool seek(long finalPos, bool relativeMovement = false);

                long getSize() const;

                long getPos() const;

                const irr::io::path& getFileName() const;
            // Private Members
            private:
                const std::string mFileName;
                PHYSFS_File *mFileHandle;
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_FILEREADOBJECT_HPP_
