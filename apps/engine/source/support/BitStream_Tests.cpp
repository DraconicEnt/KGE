/**
 *  @file BitStream_Tests.cpp
 *  @brief Source file containing coding for the BitStream tests.
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
        // BitStream GTest Code
        #if ENGINE_TESTS>0
            #include <gtest/gtest.h>

            const static Kiaro::Common::F32 sFloatList[] =
            {
                3.14159,
                1337.125,
            };

            const static Kiaro::Common::U32 sFloatCount = sizeof(sFloatList) / sizeof(Kiaro::Common::F32);

            const static Kiaro::Common::String sStringList[] =
            {
                "String One",
                "String Two",
                "String Three",
            };

            const static Kiaro::Common::U32 sStringCount = sizeof(sStringList) / sizeof(Kiaro::Common::String);

            inline void PackFloats(Kiaro::Support::BitStream &destination)
            {
                for (Kiaro::Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                    destination.write<Kiaro::Common::F32>(sFloatList[iteration]);
            }

            TEST(BitStream, Floats)
            {
                Kiaro::Common::U32 expectedStreamSize = sFloatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Support::BitStream floatStream(expectedStreamSize);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
                EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Kiaro::Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                    EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], floatStream.read<Kiaro::Common::F32>());

                // Reset and read using memcpy
                floatStream.mDataPointer = floatStream.getSize() - 1;

                for (Kiaro::Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                {
                    const Kiaro::Common::F32 &currentValue = floatStream.read<Kiaro::Common::F32>();
                    EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], currentValue);

                    // Deallocate without dying now?
                    //delete &currentValue;
                }
            }

            TEST(BitStream, BadStartFloats)
            {
                Kiaro::Common::U32 expectedStreamSize = sFloatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Support::BitStream floatStream(expectedStreamSize - 3);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
                EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Kiaro::Common::S32 iteration = sFloatCount - 1; iteration > -1; iteration--)
                    EXPECT_EQ(sFloatList[iteration], floatStream.read<Kiaro::Common::F32>());
            }

            TEST(BitStream, Vector)
            {
                Kiaro::Common::U32 expectedStreamSize = sFloatCount * sizeof(Kiaro::Common::F32);

                Kiaro::Common::Vector3DF testVector(1.0f, 2.0f, 3.0f);
                Kiaro::Support::BitStream vectorStream(expectedStreamSize);
                vectorStream.write<Kiaro::Common::Vector3DF>(testVector);

                const Kiaro::Common::Vector3DF &readVector = vectorStream.read<Kiaro::Common::Vector3DF>();

                // Are the components correct?
                EXPECT_EQ(1.0f, readVector.X);
                EXPECT_EQ(2.0f, readVector.Y);
                EXPECT_EQ(3.0f, readVector.Z);

                // Kill it
                //delete &readVector;
            }

            TEST(BitStream, String)
            {
                // Write and read a singular String
                Kiaro::Support::BitStream singleStringTest;

                singleStringTest.writeString(sStringList[0]);
                EXPECT_TRUE(singleStringTest.readString().compare(sStringList[0]) == 0);

                // Calculate the expected stream size
                Kiaro::Common::U32 expectedStreamSize = 0;

                for (Kiaro::Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                    expectedStreamSize += sStringList[iteration].length();

                // Pack them
                Kiaro::Support::BitStream testStream;
                for (Kiaro::Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                    testStream.writeString(sStringList[iteration]);

                // Unpack
                for (Kiaro::Common::S32 iteration = sStringCount - 1; iteration > -1; iteration--)
                {
                    std::string current = testStream.readString();

                    std::cout << "Got: " << current << std::endl;
                    std::cout << "Needed: " << sStringList[iteration] << std::endl;
                    EXPECT_TRUE(current.compare(sStringList[iteration]) == 0);
                }
            }
        #endif // _INCLUDE_KIARO_TESTS_H_
    } // End Namespace Support
} // End namespace Kiaro
