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
        CBitStream::CBitStream(ISerializable* in) : mTotalSize(in->getRequiredMemory()),
        mMemoryBlock(new Common::U8[in->getRequiredMemory()]), mPointer(0), mOwnsMemoryBlock(true)
        {
            in->packEverything(*this);
        }

        CBitStream::CBitStream(void* initializer, const size_t &initializerLength) : mMemoryBlock((Common::U8*)initializer),
        mTotalSize(initializerLength), mPointer(0), mOwnsMemoryBlock(false)
        {

        }

        CBitStream::CBitStream(const size_t& sizeInBytes, const void* initializer, size_t initializerLength) :
        mMemoryBlock(new Common::U8[sizeInBytes]), mPointer(0), mTotalSize(sizeInBytes), mOwnsMemoryBlock(true)
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

        void CBitStream::writeString(const Common::C8* string, const size_t& length)
        {
            // FIXME: The check below may cause a SIGSEGV if we're out on the heap

            // Is it properly NULL terminated?
            if (string[length] != 0x00)
                throw std::runtime_error("Attempted to write bad string!");

            const size_t stringLength = strlen(string) + 1; // Account for the NULL as well

            if (stringLength - 1 != length)
                throw std::runtime_error("Attempted to write a bad string!");

            // Will the string fit?
            if (mTotalSize - mPointer < stringLength + sizeof(size_t))
                throw std::runtime_error("Cannot fit string into bit stream buffer!");

            // Write off the string length so we can properly unpack later
            this->write<size_t>(stringLength);

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
            const size_t& totalBytes = this->pop<size_t>();

            //if (mPointer <= 0 || mPointer < totalBytes)
              //  throw std::underflow_error("Stack Underflow");

            const Common::C8* result = reinterpret_cast<const Common::C8*>(&mMemoryBlock[mPointer]);

            mPointer += totalBytes;
            return result;
        }

        const Common::C8* CBitStream::topString(void)
        {
            // Read the string length off
            const size_t& totalBytes = this->pop<size_t>();

            // First off, is there enough memory in the buffer?
         //   if (totalBytes > mPointer)
            //    throw std::underflow_error("Stack Underflow");

            // Ensure that the string is properly terminated
           // if (mMemoryBlock[(mPointer - sizeof(size_t)) - 1] != 0x00)
            //    throw std::logic_error("Attempted to unpack an improperly terminated string");

            // Return the result
           return reinterpret_cast<const Common::C8*>(&mMemoryBlock[mPointer]);
        }

        const size_t& CBitStream::getPointer(void)
        {
            return mPointer;
        }

        void* CBitStream::getBlock(void)
        {
            return mMemoryBlock;
        }

        void CBitStream::setPointer(const size_t& pointer)
        {
            if (pointer < 0 || pointer >= mTotalSize)
                throw std::out_of_range("Attempted to index out of bounds in BitStream");

            mPointer = pointer;
        }

        void CBitStream::resize(const size_t& newSize)
        {
            Common::U8* newBlock = new Common::U8[newSize];
            memset(newBlock, 0x00, newSize);

            // Determine how much memory from our old block to copy.
            const size_t copyLength = newSize < mTotalSize ? mTotalSize - newSize : mTotalSize;

            // Copy the old memory block and delete it.
            memcpy(newBlock, mMemoryBlock, copyLength);

            // Only delete our memory block if we actually own it.
            if (mOwnsMemoryBlock)
                delete[] mMemoryBlock;

            // Update our pointer and size.
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
            return mTotalSize >= mPointer;
        }

        bool CBitStream::isEmpty(void) const
        {
            return mPointer >= mTotalSize;
        }

        void CBitStream::write(const ISerializable *in)
        {
            in->packEverything(*this);
        }

        const size_t& CBitStream::getSize(void) const
        {
            return mTotalSize;
        }
    } // End Namespace Support
} // End namespace Kiaro
