/**
 *  @file CBitStream.h
 *  @brief Include file defining the Kiaro::Support::CBitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_CBITSTREAM_H_
#define _INCLUDE_KIARO_SUPPORT_CBITSTREAM_H_

#include <type_traits>  // std::is_pointer
#include <exception>

#include <physfs.h>

#include <support/FEndian.hpp>
#include <support/common.hpp>
#include <support/String.hpp>

namespace Kiaro
{
    namespace Support
    {
        class ISerializable;

        /**
         *  @brief A CBitStream is a class type with which you can use to stream sequences of data to memory blocks
         *  and back. This is mostly used by the netcode which is used to pack data in a system agnostic manner
         *  for transmission across the network to remote systems.
         */
        class CBitStream
        {
            // Private members
            private:
                //! A pointer to the start of the contiguous memory block this bit stream is working with.
                Common::U8* mMemoryBlock;

                //! The current pointer into the memory block. This is used to track where reads and writes will occur.
                size_t mPointer;

                //! The total size of the memory block that this bit stream is working with.
                size_t mTotalSize;

                //! A boolean representing whether or not this bit stream owns the memory block pointed to by mMemoryBlock.
                bool mOwnsMemoryBlock;

                //! When a resize is necessary, this is the number of bytes we will extend the buffer length by.
                size_t mResizeLength;

            // Public Members
            public:
                //! Whether or not the endianness of written and read data should be reversed. This is useful for bit streams that are writing to the network.
                bool mInverseEndian;

            // Public methods
            public:
                /**
                 *  @brief A constructor accepting a pointer to an ISerializable object to pack into this
                 *  stream.
                 *  @param in A pointer to the ISerializable object of choice.
                 */
                CBitStream(ISerializable* in);

                /**
                 *  @brief A constructor accepting an initializer block of memory. When constructed this way,
                 *  this bit stream does not own the memory block, therefore its up to code elsewhere to properly
                 *  deallocate this memory.
                 *  @param initializer A pointer to the memory to initialize with.
                 *  @param initializerLength The total size of memory in bytes the initializer is.
                 *  @param resizeLength The number of bytes that this bit stream will expand by if the existing buffer is exceeded.
                 */
                CBitStream(void* initializer, const size_t initializerLength, const size_t resizeLength = 0);

                CBitStream(const size_t sizeInBytes, const void* initializer = nullptr, size_t initializerLength = 0, const size_t resizeLength = 0);

                //! Standard destructor.
                ~CBitStream(void);

                /**
                 *  @brief Templated method that allows for the writing of arbitrary primitive types to this bit stream.
                 *  @param input The input data to write to this bit stream.
                 */
                template <typename inType>
                void write(const inType& input)
                {
                    static_assert(!std::is_pointer<inType>::value, "Cannot write pointer values to a bit stream!");

                    assert(mPointer <= mTotalSize);

                    if ((mPointer >= mTotalSize || mTotalSize - mPointer < sizeof(inType)) && mResizeLength == 0)
                        throw std::overflow_error("Stack Overflow");
                    else if (mPointer >= mTotalSize || mTotalSize - mPointer < sizeof(inType))
                        this->resize(mTotalSize + mResizeLength);

                    inType& output = *reinterpret_cast<inType*>(&mMemoryBlock[mPointer]);
                    output = input;

                    // Resolve the endian swap if we need to
                    if (mInverseEndian)
                        SwapEndian(output);

                    mPointer += sizeof(inType);
                }

                /**
                 *  @brief Writes an arbitrary ISerializable object to the stream for later unpacking.
                 *  @param in A pointer to the input ISerializable object to pack.
                 */
                void write(const ISerializable* in);

                /**
                 *  @brief Writes a string to the CBitStream, allowing for later unpacking.
                 *  @param string The string to write into the CBitStream.
                 */
                void writeString(const std::string& string);

                /**
                 *  @brief Writes a regular C string to the CBitStream, allowing for later unpacking.
                 *  @param string THe string to write into the CBitStream.
                 *  @param length How long the string is, in bytes excluding the NULL byte.
                 */
                void writeString(const Common::C8* string, const size_t length);

                /**
                 *  @brief Returns a pointer to the string that is currently at the top of the CBitStream.
                 *  @return A pointer to the raw C string that is currently at the top of the CBitStream.
                 */
                const Common::C8* topString(void);

                /**
                 *  @brief Returns a pointer to the string that is currently at the top of the CBitStream while also
                 *  popping it from the CBitStream.
                 *  @return A pointer to the raw C string that is currently at the top of the CBitStream at the time of
                 *  calling.
                 */
                const Common::C8* popString(void);

                /**
                 *  @brief Returns a reference to the next primitive value in the stream while also making
                 *  it the new top of the stream.
                 *  @details Data that that was once previously at the top is untouched as the internal state is just a
                 *  pointer into the memory block that's advanced for each pop and write.
                 */
                template <typename outType>
                outType& pop(void)
                {
                    if (mPointer >= mTotalSize)
                        throw std::underflow_error("Stack Underflow");

                    outType& result = this->top<outType>();

                    // NOTE: We will rewrite the actual value in the stream when we do this, but this should only be read once anyway
                    if (mInverseEndian)
                        SwapEndian(result);

                    mPointer += sizeof(outType);
                    return result;
                }

                /**
                 *  @brief Returns a reference to what is currently at the top of the stream without modifiying the
                 *  internal stream state.
                 *  @return A reference to an arbitrary primitive value desiginated by the template parameter read
                 *  from the top of the stream.
                 */
                template <typename outType>
                outType& top(void) const
                {
                    if (mPointer + sizeof(outType) > mTotalSize)
                        throw std::underflow_error("Stack Underflow");

                    return *reinterpret_cast<outType*>(&mMemoryBlock[mPointer]);
                }

                /**
                 *  @brief Returns whether or not this bit stream cannot hold any more data in its reserved memory block.
                 *  @return A boolean representing whether or not the bit stream currently has room to store more data in
                 *  its memory block.
                 */
                bool isFull(void) const;

                /**
                 *  @brief Returns whether or not this bit stream has nothing in it.
                 *  @return A boolean representing whether or not the bit stream currently is not storing any sort of data
                 *  in its memory block.
                 */
                bool isEmpty(void) const;

                /**
                 *  @brief Resizes the internal block of memory to the given size.
                 *  @param newSize The new size to use.
                 *  @note If this bit stream was given an initializer block of memory at construction, it simply forgets about
                 *  that block which can mean there's a potential memory leak if no other code ever deallocates that block.
                 *  @warning Data will be destroyed if the new size is less than the the data pointer the bit stream is
                 *  currently using.
                 */
                void resize(const size_t newSize);

                /**
                 *  @brief Templated operator overload that allows one to use the stream insertion operator for writing
                 *  arbitrary primitive data into the bit stream.
                 *  @param stream The left hand bit stream to write into.
                 *  @param input The right hand input primitive to write.
                 *  @return A reference to the original input stream so that the streaming can continue.
                 */
                template <typename inType>
                friend CBitStream& operator <<(CBitStream& stream, const inType& input)
                {
                    stream.write(input);
                    return stream;
                }

                /**
                 *  @brief Templated operator overload that allows one to use the stream extraction operator for reading
                 *  arbitrary primitive data from the bit stream.
                 *  @param stream The left hand bit stream to read from.
                 *  @param input The right hand input primitive to read into.
                 *  @return A reference to the original input stream so that the streaming can continue.
                 */
                template <typename outType>
                friend CBitStream& operator >>(CBitStream& stream, outType& output)
                {
                    output = stream.top<outType>();
                    stream.pop<outType>();
                    return stream;
                }

                template <typename serializableType>
                friend CBitStream& operator >>(CBitStream& stream, serializableType*& output)
                {
                    output = stream.pop<serializableType>();
                    return stream;
                }

                /**
                 *  @brief Returns a constant reference to the stream's memory block pointer index. This is not to be
                 *  confused with the actual memory block pointer, this value represents where in the internal memory
                 *  block the bit stream currently is at.
                 *  @return A reference to the internally stored stream index pointer.
                 */
                size_t getPointer(void);

                /**
                 *  @brief Returns a pointer to the internal memory block that this bit stream is using.
                 *  @return A pointer to the internal memory block.
                 */
                void* getBlock(void);

                /**
                 *  @brief Sets the current location of the stream pointer.
                 *  @param pointer The new pointer value to use.
                 */
                void setPointer(const size_t pointer);

                size_t getSize(void) const;
        };

        /**
         *  @brief An interface class representing a non-primitive type that may be packed and unpacked from an arbitrary
         *  bit stream.
         */
        class ISerializable
        {
                // Public methods
            public:
                //! Parameter-less constructor.
                ISerializable(void);

                /**
                 *  @brief Constructor accepting an input bit stream.
                 *
                 *  @details When constructed in this manner, the ISerializable is to unpack a full set of instantiation data from
                 *  the stream which is promptly written to the appropriate member fields. This would be the full set of data that is
                 *  initially sent to a remote client to notify about the existence of a given game object.
                 *  @param in The input bit stream.
                 */
                ISerializable(Support::CBitStream& in)
                {
                    this->unpack(in);
                }

                /**
                 *  @brief Packs all data about this ISerializable into the CBitStream. This is usually required for
                 *  new instantiations of objects.
                 *  @param out The CBitStream to write into.
                 */
                virtual void packEverything(Support::CBitStream& out) const = 0;

                /**
                 *  @brief Unpacks a payload from the current position in the input bit stream for this
                 *  ISerializable object.
                 *  @param in The input CBitStream to unpack from.
                 */
                virtual void unpack(Support::CBitStream& in) = 0;

                /**
                 *  @brief Returns the total number of bytes required to pack this ISerializable object
                 *  into a CBitStream.
                 *  @return The number of bytes required to have left in a CBitStream to be able to pack
                 *  this ISerializable.
                 */
                virtual size_t getRequiredMemory(void) const = 0;
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_
