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
        mAllocatedBytes(inDataLength), mDataPointer(inDataLength - 1), mCurrentBit(0), mBooleanMode(false), mWrittenBytes(inDataLength)
        {
            mData = new Kiaro::Common::U8[inDataLength];
            memcpy(mData, inData, inDataLength);
        }

       // BitStream::BitStream(const Kiaro::Common::U8 *initialData, size_t initialDataLength, size_t initialDataIndex)
       // {
        //    BitStream((Kiaro::Common::U8*)initialData);
        //    mIsManagingMemory = false; // NOTE (Robert MacGregor#1): Gets Overwritten by the Constructor Call Above
       // }

       // BitStream::BitStream(size_t allocationSize) : mIsManagingMemory(true), mData(new Kiaro::Common::U8[initialDataLength]),
       // mDataPointer(0), mAllocatedBytes (initialDataLength)
       // {

       // }

        BitStream::~BitStream(void)
        {
            if (mData)
                delete mData;

            mData = NULL;
        }

        Kiaro::Common::String BitStream::readString(void)
        {
            // We should be sitting on a NULL terminator already
            if (mData[mDataPointer] != 0x00)
                return "DERP - ONE";

            mDataPointer--;

            // Okay, now we need to loop backwards until we hit another NULL terminator or the end
        	for (Kiaro::Common::S32 iteration = mDataPointer; iteration > -1; iteration--)
				if (mData[iteration] == 0x00 || iteration == 0)
				{
                    const size_t difference = mDataPointer - iteration;
                    mDataPointer -= difference;

                    //if (mData[iteration] == 0x00)
                    //    mDataPointer++;

                    Kiaro::Common::String result((const Kiaro::Common::C8 *)(mData + mDataPointer));

                    std::cout << "GOT: " << result << std::endl;
                    //return result;
                }

            for (Kiaro::Common::S32 iteration = mWrittenBytes - 1; iteration > -1; iteration--)
                printf("%u: %c (0x%x)\n", iteration, mData[iteration], mData[iteration]);

            //std::cout << "HUH" << std::endl;

            return "DERP - TWO";
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
