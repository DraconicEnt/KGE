/**
 *  @file BitStream.cpp
 *  @brief BitStream testing implementation.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/Config.hpp>

#if ENGINE_TESTS>0
    #ifndef _INCLUDE_KIARO_TESTS_BITSTREAM_H_
        #define _INCLUDE_KIARO_TESTS_BITSTREAM_H_

        #include <gtest/gtest.h>

        #include <engine/Common.hpp>
        #include <support/BitStream.hpp>

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
            EXPECT_EQ(expectedStreamSize, floatStream.length());

            // Now make sure we can unpack the data correctly
            for (Kiaro::Common::U32 iteration = 0; iteration < floatCount; iteration++)
                EXPECT_EQ(float_list[(floatCount - 1) - iteration], floatStream.read<Kiaro::Common::F32>());

            // Reset and read using memcpy
            floatStream.mDataPointer = floatStream.length();

            for (Kiaro::Common::U32 iteration = 0; iteration < floatCount; iteration++)
            {
                Kiaro::Common::F32 &currentValue = floatStream.read<Kiaro::Common::F32>(true);
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
            EXPECT_EQ(expectedStreamSize, floatStream.length());

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
#endif // ENGINE_TESTS
