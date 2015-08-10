/**
 *  @file FileBitStream.hpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_
#define _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_

#include <support/support.hpp>
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

        class FileBitStream
        {
            // Public Members
            public:
                FileBitStream(const Support::String &fileName, const BITSTREAM_MODE &fileMode);
                ~FileBitStream(void);

                template <typename returnType>
                returnType copy(void)
                {
                    if (mFileHandle)
                    {
                        returnType result;
                        const size_t outDataLength = sizeof(returnType);

                        //if (outDataLength > mDataPointer)
                        //    throw std::out_of_range("FileBitStream: Attempted to read out of range");

                        //mDataPointer -= outDataLength;
                        PHYSFS_read(mFileHandle, &result, outDataLength, 1);

                        return result;
                    }

                    throw std::logic_error("FileBitStream: Attempted to copy without a file handle!");
                }

            // Private Members
            private:
                PHYSFS_File *mFileHandle;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_FILEBITSTREAM_HPP_
