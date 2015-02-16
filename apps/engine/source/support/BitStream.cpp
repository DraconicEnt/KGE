/**
 *  @file BitStream.cpp
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

#include <support/BitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        BitStream::BitStream(const size_t &initialDataIndex, const size_t &allocationSize) : mAllocationSize(allocationSize),
                             mAllocatedBytes(allocationSize), mDataPointer(initialDataIndex), mCurrentBit(0), mBooleanMode(false),
                             mWrittenBytes(0)
        {
            mData = new Kiaro::Common::U8[allocationSize];
        }

        BitStream::BitStream(const void *inData, const size_t &inDataLength, const size_t &allocationSize) : mAllocationSize(allocationSize),
        mAllocatedBytes(inDataLength), mDataPointer(inDataLength), mCurrentBit(0), mBooleanMode(false), mWrittenBytes(inDataLength)
        {
            mData = new Kiaro::Common::U8[inDataLength];
            memcpy(mData, inData, inDataLength);
        }

        BitStream::~BitStream(void)
        {
            if (mData)
                delete mData;

            mData = NULL;
        }

        Kiaro::Common::String BitStream::readString(void)
        {
            // Unpack the length
            const Kiaro::Common::U32 nextLength = this->read<Kiaro::Common::U32>();

            if (nextLength > mAllocatedBytes - mDataPointer)
                throw std::logic_error("BitStream: Cannot unpack String; not enough data!");

            mDataPointer -= nextLength;

            Kiaro::Common::String result((const Kiaro::Common::C8 *)(mData + mDataPointer), nextLength);
            return result;
        }

        void *BitStream::getBasePointer(void)
        {
            return mData;
        }

        void *BitStream::raw(const bool &shouldMemcpy)
        {
            return mData;
        }
    } // End Namespace Support
} // End namespace Kiaro
