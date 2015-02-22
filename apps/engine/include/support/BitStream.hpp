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

#include <physfs.h>

#include <engine/Common.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;

        class SerializableObjectBase
        {
            // Public Methods
            public:

                virtual void packData(Kiaro::Support::BitStream &out) = 0;
                virtual void unpackData(Kiaro::Support::BitStream &in) = 0;
        };

        //! BitStreams are serialized chunks of data that allow for easy read/write operations.
        class BitStream
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting an initial array and length.
                 *  @param initialData A pointer to a Kiaro::U8 array that utilize as our initial memory.
                 *  @param initialDataLength The size in bytes that our in parameter is, if specified.
                 *  @param initialDataIndex The starting index in the initialData.
                 *  @warning If initial_data_array is specified, it may be deallocated if the BitStream allocates
                 *  more memory when attempting to write beyond its current bounds.
                 *  @warning When using this constructor, The Kiaro::Support::BitStream code will delete the input
                 *  array upon deletion of itself or if it needs to readjust its array length.
                 */
                BitStream(const size_t &initialDataIndex = 0, const size_t &allocationSize = 32);

                /**
                 *  @brief Constructor accepting an initial length.
                 *  @param initial_data_length A Kiaro::Common::U32 representing the initial size that this Kiaro::Support::BitStream
                 *  should allocate its internal array to be.
                 *  @warning When using this constructor, the memory associated with the Kiaro::Support:BitStream's internal array
                 *  is destroyed when resizing and/or when the Kiaro::Support::BitStream instance itself is deleted.
                 */
                BitStream(const void *inData, const size_t &inDataLength, const size_t &allocationSize = 32);

                //! Standard destructor.
                ~BitStream(void);

                // Write Methods

                /**
                 *  @brief Writes arbitrary data to the BitStream. The data is written to the Stream
                 *  via memcpy.
                 *  @param inData A pointer to the data to be copied into the BitStream.
                 *  @param inDataLength The size in bytes that our in data is.
                 */
                void writeData(const void *inData, const size_t &inDataLength)
                {
                    // Check to see if we have enough room to write this data
                    const size_t availableMemoryLength = mAllocatedBytes - mWrittenBytes;

                    if (availableMemoryLength < inDataLength)
                    {
                        // We need more memory, how much more do we need?
                        size_t necessaryMemoryLength = inDataLength - availableMemoryLength;

                        // We need to perform a new allocation; if the necessary memory length is less than our
                        // allocation size then just allocate using the allocation size.
                        if (necessaryMemoryLength <= mAllocationSize)
                            necessaryMemoryLength = mAllocationSize;
                        else // Although otherwise we will allocate more if necessary
                            necessaryMemoryLength = mAllocationSize + (necessaryMemoryLength - mAllocationSize);

                        const size_t newDataLength = mAllocatedBytes + necessaryMemoryLength;
                        Kiaro::Common::U8 *newMemory = new Kiaro::Common::U8[newDataLength];

                        if (mData)
                        {
                            memcpy(newMemory, mData, mWrittenBytes);
                            delete[] mData;
                        }

                        mData = newMemory;
                        mAllocatedBytes = newDataLength;
                    }

                    // Write the data
                    memcpy(mData + mDataPointer, inData, inDataLength);
                    mDataPointer += inDataLength;
                    mWrittenBytes += inDataLength;
                }

                /**
                 *  @brief Helper template method that allows quick writing of values into the
                 *  BitStream.
                 *  @param inData The data to write to the BitStream. The data is written to the
                 *  BitStream using the length of its type and memcpy.
                 */
                template <typename typeName>
                void write(const typeName &inData)
                {
                    this->writeData(&inData, sizeof(typeName));
                }

                /**
                 *  @brief Helper template method that overrides writing boolean values to the
                 *  BitStream. This is to ascertain that booleans are always 1 byte in the BitStream
                 *  as some compilers may use different sizes.
                 *  @param in The boolean to write to the BitStream.
                 */
                template <bool>
                void write(const bool &in)
                {
                    this->write<Kiaro::Common::U8>((Kiaro::Common::U8)in);
                }

                void write(SerializableObjectBase *in)
                {
                    in->packData(*this);
                }

                void write(const Kiaro::Common::Vector3DF &inVector)
                {
                    this->write<Kiaro::Common::F32>(inVector.X);
                    this->write<Kiaro::Common::F32>(inVector.Y);
                    this->write<Kiaro::Common::F32>(inVector.Z);
                }

                void write(const Kiaro::Common::ColorRGBA &inColor)
                {
                    this->write<Kiaro::Common::U32>(inColor.getRed());
                    this->write<Kiaro::Common::U32>(inColor.getGreen());
                    this->write<Kiaro::Common::U32>(inColor.getBlue());
                    this->write<Kiaro::Common::U32>(inColor.getAlpha());
                }

                void writeString(const Kiaro::Common::String &inData)
                {
                    this->writeData(inData.c_str(), inData.length());
                    this->write<Kiaro::Common::U32>(inData.length());
                }

                // Read Methods

                /**
                 *  @brief Reads arbitrary data from the BitStream and returns a reference to it.
                 *  @return A reference to the data that has been read.
                 *  @note The returned value from this method is a reference to the read data
                 *  somewhere within the block of data associated with this BitStream. Therefore
                 *  if it is modified then these changes will be reflected in the block of memory.
                 *  @throws std::out_of_range Thrown when a read operation requires for there to be
                 *  more unread bytes than there is actually available.
                 */
                template <typename returnType>
                returnType &read(void)
                {
                    const size_t outDataLength = sizeof(returnType);

                    if (outDataLength > mDataPointer)
                        throw std::underflow_error("BitStream: Attempted to read out of range!");

                    mDataPointer -= outDataLength;
                    return *((returnType *)(mData + mDataPointer));
                }

                void *readData(const size_t &outDataLength)
                {
                    if (outDataLength > mDataPointer)
                        throw std::underflow_error("BitStream: Attempted to read out of range!");

                    mDataPointer -= outDataLength;
                    return (void *)(mData + mDataPointer);
                }

                // Copy Methods

                void copyData(void *destination, const size_t &outDataLength)
                {
                    if (outDataLength > mDataPointer)
                        throw std::underflow_error("BitStream: Attempted to copy out of range!");
                    else
                        mDataPointer -= outDataLength;

                    //if (mFileHandle)
                    //    PHYSFS_read(mFileHandle, destination, outDataLength, 1);
                    //else
                        memcpy(destination, mData + mDataPointer, outDataLength);
                }

                template <typename returnType>
                returnType copy(void)
                {
                    return this->read<returnType>();
                }

                template <typename indexType = Kiaro::Common::C8>
                const indexType &operator[](const Kiaro::Common::U32 &index)
                {
                    return *((indexType*)&mData[index]);
                }

                void read(SerializableObjectBase *copyTo)
                {
                    copyTo->unpackData(*this);
                }

                void read(Kiaro::Common::Vector3DF &out)
                {
                    out.Z = read<Kiaro::Common::F32>();
                    out.Y = read<Kiaro::Common::F32>();
                    out.X = read<Kiaro::Common::F32>();
                }

                /**
                 *  @brief Reads a Kiaro::C8 string from the BitStream.
                 *  @param outStringLength The total number of bytes to read for the string.
                 *  @param should_memcpy A boolean representing whether or not the data should be copied.
                 *  @return The next Kiaro::c8 in the BitStream.
                 *  @see BitStream::Read
                 */
                Kiaro::Common::String readString(void);

                void *getBasePointer(void) NOTHROW;

                /**
                 *  @brief Returns the void pointer representation of the BitStream's contents.
                 *  @return A void pointer to the start of the BitStream's contents.
                 */
                void *raw(const bool &shouldMemcpy = false);

                template <typename indexType = Kiaro::Common::C8>
                size_t getSize(void) { return mWrittenBytes / sizeof(indexType); }

                const size_t &getDataPointer(void) { return mDataPointer; }

            // Public Members
            public:
                //! Keeps track of where we are currently in the BitStream during read and write operations.
                size_t mDataPointer;

            // Private Members
            private:
                //! The array where all BitStream data is read/written from.
                Kiaro::Common::U8 *mData;

                //! The current size of the BitStream in bytes.
                size_t mAllocatedBytes;
                size_t mWrittenBytes;
                size_t mAllocationSize;

                // TODO (Robert MacGregor#9): Boolean group compression
                //! Represents the current bit that the BitStream is currently on. This is only used when reading and writing multiple booleans in a row.
                Kiaro::Common::U8 mCurrentBit;
                //! Represents if the BitStream is currently operating with booleans.
                bool mBooleanMode;
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_
