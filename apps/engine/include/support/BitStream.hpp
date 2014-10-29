/**
 *  @file BitStream.h
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_
#define _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_

#include <boost/iostreams/stream_buffer.hpp>
#include <boost/filesystem/fstream.hpp>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        //! BitStreams are serialized chunks of data that allow for easy read/write operations.
        class BitStream
        {
            public:
                /**
                 *  @brief Constructor accepting an initial array and length.
                 *  @param initial_data_array A pointer to a Kiaro::u8 array that utilize as our initial memory.
                 *  @param initial_data_length The size in bytes that our in parameter is, if specified.
                 *  @warning If initial_data_array is specified, it may be deallocated if the BitStream allocates
                 *  more memory when attempting to write beyond its current bounds.
                 *  @warning When using this constructor, The Kiaro::Support::BitStream code will delete the input
                 *  array upon deletion of itself or if it needs to readjust its array length.
                 */
                BitStream(Kiaro::Common::U8 *initialData = NULL, size_t initialDataLength = 0, size_t initialDataIndex = 0);

                /**
                 *  @brief Constructor accepting a constant initial array and length.
                 *  @param initial_data_array A pointer to a Kiaro::u8 array that utilize as our initial memory.
                 *  @param initial_data_length The size in bytes that our in parameter is, if specified.
                 *  @warning If initial_data_array is specified, it may be deallocated if the BitStream allocates
                 *  more memory when attempting to write beyond its current bounds.
                 *  @note In contrary to the other constructor accepting an initial array, this puts the burden
                 *  of making sure the memory associated with the input array upon you rather than Kiaro::Support::BitStream
                 *  code. It will not delete the array under any circumstance.
                 */
                //BitStream(const Kiaro::Common::U8 *initialData = NULL, size_t initialDataLength = 0, size_t initialDataIndex = 0);

                /**
                 *  @brief Constructor accepting an initial length.
                 *  @param initial_data_length A Kiaro::Common::u32 representing the initial size that this Kiaro::Support::BitStream
                 *  should allocate its internal array to be.
                 *  @warning When using this constructor, the memory associated with the Kiaro::Support:BitStream's internal array
                 *  is destroyed when resizing and/or when the Kiaro::Support::BitStream instance itself is deleted.
                 */
                BitStream(size_t initialDataLength);

                //! Standard destructor.
                ~BitStream(void);

                /**
                 *  @brief Writes arbitrary data to the BitStream. All written data is copied with memcpy -- the original
                 *  source may be destroyed afterwards.
                 *  @param in_data A pointer to the data to be copied into the bitstream.
                 *  @param in_data_length The size in bytes that our in data is.
                 */
                void write(const void *inData, const size_t &inDataLength);

                /**
                 *  @brief Writes an Kiaro::f32 to the BitStream.
                 *  @param in_data An Kiaro::f32 to write to the BitStream.
                 *  @see BitStream::Write
                 */
                void writeF32(const Kiaro::Common::F32 &inData);

                /**
                 *  @brief Writes a bool to the BitStream.
                 *  @param in_data A bool to write to the BitStream.
                 *  @see BitStream::Write
                 */
                void writeBool(const bool &inData);

                /**
                 *  @brief Writes a Kiaro::u8 to the BitStream.
                 *  @param in_data A Kiaro::u8 to write to the BitStream.
                 *  @see BitStream::Write
                 */
                 void writeU8(const Kiaro::Common::U8 &inData);

                /**
                 *  @brief Writes a Kiaro::u32 to the BitStream.
                 *  @param in_data A Kiaro::u32 to write to the BitStream.
                 *  @see BitStream::Write
                 */
                 void writeU32(const Kiaro::Common::U32 &inData);

                /**
                 *  @brief Writes a NULL terminated Kiaro::c8 string to the BitStream.
                 *  @param in_string A NULL terminated Kiaro::c8 string to write to the BitStream.
                 *  @see BitStream::Write
                 */
                void writeString(const std::string &inString);

                /**
                 *  @brief Reads arbitrary data from the BitStream.
                 *  @param out_data_length The number in bytes to read.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied. If true,
                 *  memcpy is used to create a new pointer that may be manipulated at will -- even if the source
                 *  BitStream is destroyed.
                 *  @return A void* pointer representing the data in the BitStream.
                 *  @warning If should_memcpy is true, the data returned must be manually deallocated.
                 *  @throws std::out_of_range Thrown when a read operation goes out of bounds.
                 */
                void *read(const size_t &outDataLength, const bool &shouldMemcpy = false);

                /**
                 *  @brief Reads a Kiaro::f32 from the BitStream.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::f32 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::F32 readF32(const bool &shouldMemcpy = false);

                /**
                 *  @brief Reads a bool from the BitStream.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next bool in the BitStream.
                 *  @see BitStream::Read
                 */
                bool readBool(const bool &shouldMemcpy = false);

                /**
                 *  @brief Reads a Kiaro::u8 from the BitStream.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::u8 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::U8 readU8(const bool &shouldMemcpy = false);

                /**
                 *  @brief Reads a Kiaro::u32 from the BitStream.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::u8 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::U32 readU32(const bool &shouldMemcpy = false);

                /**
                 *  @brief Reads a Kiaro::c8 string from the BitStream.
                 *  @param out_string_length The total number of bytes to read for the string.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::c8 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::C8 *readString(const size_t &outStringLength = 0, const bool &shouldMemcpy = false);

                /**
                 *  @brief Returns the void pointer representation of the BitStream's contents.
                 *  @return A void pointer to the start of the BitStream's contents.
                 */
                void *raw(const bool &shouldMemcpy = false);

                size_t length(void);

            private:
                //! The array where all BitStream data is read/written from.
                Kiaro::Common::U8 *mData;

                //! Keeps track of where we are currently in the BitStream during read and write operations.
                size_t mDataPointer;
                //! The current size of the BitStream in bytes.
                size_t mDataLength;

                //! A boolean representing whether or not this Kiaro::Support::BitStream is the sole manager of the associated memory.
                bool mIsManagingMemory;

                boost::filesystem::ofstream *mOutFileStream;
                boost::filesystem::ifstream *mInFileStream;
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_
