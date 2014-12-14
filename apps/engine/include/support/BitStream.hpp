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

#include <type_traits>  // std::is_pointer
#include <exception>

#include <boost/iostreams/stream_buffer.hpp>
#include <boost/filesystem/fstream.hpp>

#include <engine/Common.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        //! BitStreams are serialized chunks of data that allow for easy read/write operations.
        class BitStream
        {
            // Public Methods
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
                //void write(const void *inData, const size_t &inDataLength);

                template <typename typeName>
                void write(const typeName &inData, size_t inDataLength = 0)
                {
                    if (inDataLength == 0)
                        inDataLength = sizeof(typeName);
                    else if (!std::is_pointer<typeName>::value)
                        throw std::logic_error("BitStream: Attempted to use non-sizeof data length with a non-pointer!");

                    // Check to see if we have enough room to write this data
                    if (mDataLength < mDataPointer + inDataLength)
                    {
                        // Technically should be zero ...
                        const Kiaro::Common::U32 dataLengthOffset = mDataLength - mDataPointer;

                        Kiaro::Common::U32 newDataLength = (mDataLength - dataLengthOffset) + inDataLength;
                        Kiaro::Common::U8 *newMemory = new Kiaro::Common::U8[newDataLength];

                        memcpy(newMemory, mData, mDataLength);
                        mDataLength = newDataLength;

                        if (mData && mIsManagingMemory)
                            delete mData;

                        mData = newMemory;
                        mIsManagingMemory = true;
                    }

                    memcpy(&mData[mDataPointer], &inData, inDataLength);
                    mDataPointer += inDataLength; // NOTE (Robert MacGregor#9): Preserves the offset
                }

                template <bool>
                void write(const bool &in)
                {
                    write<Kiaro::Common::U8>(in);
                }

                void write(const Kiaro::Common::Vector3DF &inVector)
                {
                    write<Kiaro::Common::F32>(inVector.X);
                    write<Kiaro::Common::F32>(inVector.Y);
                    write<Kiaro::Common::F32>(inVector.Z);
                }

                void write(const Kiaro::Common::ColorRGBA &inColor)
                {
                    write<Kiaro::Common::U32>(inColor.getRed());
                    write<Kiaro::Common::U32>(inColor.getGreen());
                    write<Kiaro::Common::U32>(inColor.getBlue());
                    write<Kiaro::Common::U32>(inColor.getAlpha());
                }

                void write(const std::string &inData)
                {
                    write(inData.c_str(), inData.length() + 1);
                }

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
                template <class returnType>
                returnType &read(const bool &shouldMemcpy = false)
                {
                    const size_t outDataLength = sizeof(returnType);

                    if (outDataLength > mDataPointer)
                        throw std::out_of_range("BitStream attempted to read out of range!");
                    else
                        mDataPointer -= outDataLength;

                    if (shouldMemcpy)
                    {
                        void *memory = (void*)new Kiaro::Common::U8[outDataLength];
                        memcpy(memory, &mData[mDataPointer], outDataLength);
                        return *((returnType*)memory);
                    }

                    return *((returnType*)&mData[mDataPointer]);
                }

                /**
                 *  @brief Reads a Kiaro::c8 string from the BitStream.
                 *  @param out_string_length The total number of bytes to read for the string.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::c8 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::C8 *readString(const size_t &outStringLength = 0, const bool &shouldMemcpy = false);

                void *getBasePointer(void) NOTHROW;

                /**
                 *  @brief Returns the void pointer representation of the BitStream's contents.
                 *  @return A void pointer to the start of the BitStream's contents.
                 */
                void *raw(const bool &shouldMemcpy = false);

                size_t length(void) NOTHROW;

            // Public Members
            public:
                //! Keeps track of where we are currently in the BitStream during read and write operations.
                size_t mDataPointer;

            // Private Members
            private:
                //! The array where all BitStream data is read/written from.
                Kiaro::Common::U8 *mData;

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
