/**
 *  @file CBitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
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
        mPointer(0), mOwnsMemoryBlock(true), mResizeLength(0)
        {
            in->packEverything(*this);
        }

        CBitStream::CBitStream(void* initializer, const size_t initializerLength, const size_t resizeLength) : mMemoryBlock((Common::U8*)initializer),
        mTotalSize(initializerLength), mPointer(0), mOwnsMemoryBlock(false), mResizeLength(resizeLength)
        {
        }

        CBitStream::CBitStream(const size_t sizeInBytes, const void* initializer, size_t initializerLength, const size_t resizeLength) :
        mMemoryBlock(new Common::U8[sizeInBytes]), mPointer(0), mTotalSize(sizeInBytes), mOwnsMemoryBlock(true), mResizeLength(resizeLength)
        {
            memset(mMemoryBlock, 0x00, sizeInBytes);

            if (initializer && !initializerLength)
                initializerLength = sizeInBytes;

            // Are we initializing from some existing memory block?
            if (initializer && initializerLength)
            {
                // Only copy what we can actually store.
                initializerLength = initializerLength >= sizeInBytes ? sizeInBytes - 1 : initializerLength;

                // Perform the copy.
                memcpy(mMemoryBlock, initializer, initializerLength);

                // Make sure we the pointer is set to the next available space
                mPointer = sizeInBytes;
            }
        }

        //! Standard destructor.
        CBitStream::~CBitStream(void)
        {
            if (mOwnsMemoryBlock)
                delete[] mMemoryBlock;
        }

        void CBitStream::writeString(const Common::C8* string, const size_t length)
        {
            assert(mPointer <= mTotalSize);

            // FIXME: The check below may cause a SIGSEGV if we're out on the heap
            // FIXME: If mResizeLength < string length, this will cause erroneous writes

            // Is it properly NULL terminated?
            if (string[length] != 0x00)
                throw std::runtime_error("Attempted to write bad string (null terminator isn't where it should be)!");

            const size_t stringLength = strlen(string) + 1; // Account for the NULL as well

            if (stringLength - 1 != length)
                throw std::runtime_error("Attempted to write a bad string (Lengths do not match)!");

            // Will the string fit?
            if (mTotalSize - mPointer < stringLength + sizeof(Common::U32) && mResizeLength == 0)
                throw std::runtime_error("Cannot fit string into buffer!");
            else if (mTotalSize - mPointer < stringLength + sizeof(Common::U32))
                this->resize(mTotalSize + mResizeLength);

            // Write off the string length so we can properly unpack later
            this->write<Common::U32>(static_cast<Common::U32>(stringLength));
            //if (mPointer >= mTotalSize || mTotalSize - mPointer < stringLength)
            //    throw std::overflow_error("Stack Overflow");
            memcpy(&mMemoryBlock[mPointer], string, stringLength);
            mPointer += stringLength;
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
                throw std::underflow_error("Stack Underflow");

            // Ensure that the string is properly terminated
            if (mMemoryBlock[mPointer + stringLength + sizeof(Common::U32) + 1] != 0x00)
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
            assert(newSize > mTotalSize);

            Common::U8* newBlock = new Common::U8[newSize];

            // Only memset the new bytes
            memset(&newBlock[mPointer + 1], 0x00, newSize - (mPointer + 1));

            // Copy any data we're actually using out of the block
            memcpy(newBlock, mMemoryBlock, mPointer);

            // Only delete our memory block if we actually own it.
            if (mOwnsMemoryBlock)
                delete[] mMemoryBlock;

            // Update our stored block information
            mMemoryBlock = newBlock;
            mTotalSize = newSize;

            // We definitely own this block now
            mOwnsMemoryBlock = true;
        }

        template <>
        void CBitStream::write(const Common::Vector3DF& input)
        {
            //if (mPointer >= mTotalSize || mTotalSize - mPointer < sizeof(inType))
            //    throw std::overflow_error("Stack Overflow");
            this->write(input.X);
            this->write(input.Y);
            this->write(input.Z);
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
    } // End Namespace Support
} // End namespace Kiaro
