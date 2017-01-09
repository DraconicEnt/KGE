/**
 *  @file CBitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#if ENGINE_TESTS>0
#include <gtest/gtest.h>
#endif // ENGINE_TESTS

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        CBitStream::CBitStream(ISerializable* in) : mTotalSize(in->getRequiredMemory()), mMemoryBlock(new Common::U8[in->getRequiredMemory()]),
        mPointer(0), mOwnsMemoryBlock(true), mResizeLength(0), mInverseEndian(false)
        {
            in->packEverything(*this);
        }

        CBitStream::CBitStream(void* initializer, const size_t initializerLength, const size_t resizeLength) : mMemoryBlock((Common::U8*)initializer),
        mTotalSize(initializerLength), mPointer(0), mOwnsMemoryBlock(false), mResizeLength(resizeLength), mInverseEndian(false)
        {
        }

        CBitStream::CBitStream(const size_t sizeInBytes, const void* initializer, size_t initializerLength, const size_t resizeLength) :
        mMemoryBlock(new Common::U8[sizeInBytes]), mPointer(0), mTotalSize(sizeInBytes), mOwnsMemoryBlock(true), mResizeLength(resizeLength),
        mInverseEndian(false)
        {
            memset(mMemoryBlock, 0x00, sizeInBytes);

            if (initializer && !initializerLength)
                initializerLength = sizeInBytes;

            // Are we initializing from some existing memory block?
            if (initializer && initializerLength)
            {
                // Only copy what we can actually store.
                initializerLength = initializerLength >= sizeInBytes ? sizeInBytes - 1 : initializerLength;
                memcpy(mMemoryBlock, initializer, initializerLength);

                // Make sure we the pointer is set to the next available space
                mPointer = sizeInBytes;
            }
        }

        //! Standard destructor.
        CBitStream::~CBitStream(void)
        {
            if (mOwnsMemoryBlock)
                free(mMemoryBlock);
        }

        void CBitStream::writeString(const Common::C8* string, const size_t length)
        {
            assert(mPointer <= mTotalSize);

            // FIXME: The check below may cause a SIGSEGV if we're out on the heap and the input string (and length) comes from an untrusted source
            // FIXME: If mResizeLength < string length, this will cause erroneous writes

            // Is it properly NULL terminated?
            if (string[length] != 0x00)
                throw std::runtime_error("Attempted to write bad string (null terminator isn't where it should be)!");

            const size_t stringLength = strlen(string);
            const size_t totalLength = stringLength + sizeof(Common::U32) + 1; // Account for the NULL as well

            if (stringLength != length)
                throw std::runtime_error("Attempted to write a bad string (Lengths do not match)!");

            // Will the string fit?
            if (mTotalSize - mPointer < totalLength && mResizeLength == 0)
                throw std::runtime_error("Cannot fit string into buffer!");
            else if (mTotalSize - mPointer < totalLength)
                this->resize(mTotalSize + mResizeLength);

            // Write off the string length so we can properly unpack later
            this->write<Common::U32>(static_cast<Common::U32>(stringLength));

            if (mPointer >= mTotalSize || mTotalSize - mPointer < totalLength)
                throw std::overflow_error("Stack Overflow");

            memcpy(&mMemoryBlock[mPointer], string, stringLength + 1);
            mPointer += stringLength + 1; // NULL byte again
        }

        void CBitStream::writeString(const std::string& string)
        {
            this->writeString(string.data(), string.length());
        }

        const Common::C8* CBitStream::popString(void)
        {
            // Grab the top string
            // FIXME: Use the length read out of the buffer instead of recalculating the string size here
            const Common::C8* result = this->topString();
            mPointer += strlen(result) + sizeof(Common::U32) + 1;

            return result;
        }

        const Common::C8* CBitStream::topString(void)
        {
            // Read the string length off
            const Common::U32 stringLength = this->top<Common::U32>();

            // First off, is there enough memory in the buffer?
            if (stringLength > mTotalSize - mPointer)
                throw std::underflow_error("Stack Underflow in String Read");

            // Ensure that the string is properly terminated
            const Common::U32 nullIndex = mPointer + stringLength + sizeof(Common::U32);
            if (mMemoryBlock[nullIndex] != 0x00)
                throw std::logic_error("Attempted to unpack an improperly terminated string");

            // Return the result
            return reinterpret_cast<const Common::C8*>(&mMemoryBlock[mPointer + sizeof(Common::U32)]);
        }

        size_t CBitStream::getPointer(void)
        {
            return mPointer;
        }

        void* CBitStream::getBlock(void)
        {
            return mMemoryBlock;
        }

        void CBitStream::setPointer(const size_t pointer)
        {
            if (pointer < 0 || pointer >= mTotalSize)
                throw std::out_of_range("Attempted to index out of bounds in BitStream");

            mPointer = pointer;
        }

        void CBitStream::resize(const size_t newSize)
        {
            // TODO: Implement as an exception
            CONSOLE_ASSERT(newSize > mTotalSize, "newSize=%u,mTotalSize=%u", newSize, mTotalSize);

            if (mOwnsMemoryBlock)
                mMemoryBlock = reinterpret_cast<Common::U8*>(realloc(mMemoryBlock, newSize));
            else
            {
                Common::U8* newBlock = reinterpret_cast<Common::U8*>(malloc(newSize));

                // Only memset the new bytes
                memset(&newBlock[mPointer + 1], 0x00, newSize - (mPointer + 1));

                // Copy any data we're actually using out of the block
                memcpy(newBlock, mMemoryBlock, mPointer);

                // Update our stored block information
                mMemoryBlock = newBlock;

                // We definitely own this block now
                mOwnsMemoryBlock = true;
            }

            mTotalSize = newSize;
        }

        template <>
        void CBitStream::write(const Support::Vector3DF& input)
        {
            if (mPointer >= mTotalSize || mTotalSize - mPointer < sizeof(Common::F32) * 3)
                throw std::overflow_error("Stack Overflow");

            this->write(input.x());
            this->write(input.y());
            this->write(input.z());
        }

        bool CBitStream::isFull(void) const
        {
            return mTotalSize == mPointer;
        }

        bool CBitStream::isEmpty(void) const
        {
            return mPointer == 0;
        }

        void CBitStream::write(const ISerializable* in)
        {
            in->packEverything(*this);
        }

        size_t CBitStream::getSize(void) const
        {
            return mTotalSize;
        }

        ISerializable::ISerializable(void) { }
    } // End Namespace Support
} // End namespace Kiaro
