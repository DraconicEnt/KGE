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
        BitStream::BitStream(Kiaro::Common::U8 *initialData, size_t initialDataLength, size_t initialDataIndex) :
                             mIsManagingMemory(true), mDataPointer(initialDataIndex), mCurrentBit(0), mBooleanMode(false)
        {
            if (initialData == NULL)
                mDataLength = 0;
            else
                mDataLength = initialDataLength;

            mIsManagingMemory = false;
            mData = initialData;
        }

       // BitStream::BitStream(const Kiaro::Common::U8 *initialData, size_t initialDataLength, size_t initialDataIndex)
       // {
        //    BitStream((Kiaro::Common::U8*)initialData);
        //    mIsManagingMemory = false; // NOTE (Robert MacGregor#1): Gets Overwritten by the Constructor Call Above
       // }

        BitStream::BitStream(size_t initialDataLength) : mIsManagingMemory(true), mData(new Kiaro::Common::U8[initialDataLength]),
        mDataPointer(0), mDataLength(initialDataLength)
        {

        }

        BitStream::~BitStream(void)
        {
            if (mIsManagingMemory)
                delete mData;
        }

        Kiaro::Common::C8 *BitStream::readString(const size_t &outStringLength, const bool &shouldMemcpy)
        {
        	// If the Length if zero, try to figure out where we should stop, then
        	// FIXME: Go out until maximum arb string length
        	//for (size_t iteration = mDataPointer; iteration < mDataLength; iteration++)
			//	if (mData[iteration] == 0x00) // Check for NULL termination
            //        return (Kiaro::Common::C8*)read<Kiaro::Common::C8>(iteration - mDataLength, shouldMemcpy);

            //return (Kiaro::Common::C8*)read(outStringLength, shouldMemcpy);
        }

        void *BitStream::getBasePointer(void)
        {
            return mData;
        }

        void *BitStream::raw(const bool &shouldMemcpy)
        {
            return mData;
        }

        // BitStream GTest Code
        #if ENGINE_TESTS>0
            #include <gtest/gtest.h>

            static Kiaro::Common::F32 float_list[] =
            {
                3.14159,
                1337.125,
            };

            static Kiaro::Common::U32 floatCount = sizeof(float_list) / sizeof(Kiaro::Common::F32);

            inline void PackFloats(Kiaro::Support::BitStream &destination)
            {
                for (Kiaro::Common::U32 iteration = 0; iteration < floatCount; iteration++)
                    destination.write<Kiaro::Common::F32>(float_list[iteration]);
            }

            TEST(BitStreamTest, Floats)
            {
                Kiaro::Common::U32 expectedStreamSize = floatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Support::BitStream floatStream(expectedStreamSize);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
                EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Kiaro::Common::U32 iteration = 0; iteration < floatCount; iteration++)
                    EXPECT_EQ(float_list[(floatCount - 1) - iteration], floatStream.read<Kiaro::Common::F32>());

                // Reset and read using memcpy
                floatStream.mDataPointer = floatStream.getSize();

                for (Kiaro::Common::U32 iteration = 0; iteration < floatCount; iteration++)
                {
                    Kiaro::Common::F32 &currentValue = floatStream.read<Kiaro::Common::F32>();
                    EXPECT_EQ(float_list[(floatCount - 1) - iteration], currentValue);

                    // Deallocate without dying now?
                    delete &currentValue;
                }
            }

            TEST(BitStreamTest, BadStartFloats)
            {
                Kiaro::Common::U32 expectedStreamSize = floatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Support::BitStream floatStream(expectedStreamSize - 3);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
                EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Kiaro::Common::S32 iteration = floatCount - 1; iteration > -1; iteration--)
                    EXPECT_EQ(float_list[iteration], floatStream.read<Kiaro::Common::F32>());
            }

            TEST(BitStreamTest, Vector)
            {
                Kiaro::Common::U32 expectedStreamSize = floatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Common::Vector3DF testVector(1.0f, 2.0f, 3.0f);
                Kiaro::Support::BitStream vectorStream(expectedStreamSize);
                vectorStream.write<Kiaro::Common::Vector3DF>(testVector);

                Kiaro::Common::Vector3DF &readVector = vectorStream.read<Kiaro::Common::Vector3DF>();

                // Are the components correct?
                EXPECT_EQ(1.0f, readVector.X);
                EXPECT_EQ(2.0f, readVector.Y);
                EXPECT_EQ(3.0f, readVector.Z);

                // Kill it
                delete &readVector;
            }
        #endif // _INCLUDE_KIARO_TESTS_H_
    } // End Namespace Support
} // End namespace Kiaro
