/**
 *  @file FileBitStream.hpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_
#define _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        enum BITSTREAM_MODE
        {
            BITSTREAM_NULL = 0,
            BITSTREAM_READ = 1,
            BITSTREAM_WRITE = 2,
            BITSTREAM_APPEND = 4,
        };

        class CFileBitStream
        {
                // Public Members
            public:
                CFileBitStream(const Support::String& fileName, const BITSTREAM_MODE& fileMode);
                ~CFileBitStream(void);

                // Private Members
            private:
                PHYSFS_File* mFileHandle;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_
