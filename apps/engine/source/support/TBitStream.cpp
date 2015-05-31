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

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        // BitStream GTest Code
        #if ENGINE_TESTS>0
            const static Common::F32 sFloatList[] =
            {
                3.14159,
                1337.125,
            };

            const static Common::U32 sFloatCount = sizeof(sFloatList) / sizeof(Common::F32);

            const static Support::String sStringList[] =
            {
                "String One",
                "String Two",
                "String Three",
            };

            const static Common::U32 sStringCount = sizeof(sStringList) / sizeof(Support::String);

            inline void PackFloats(Kiaro::Support::CBitStream &destination)
            {
                for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                    destination.write<Common::F32>(sFloatList[iteration]);
            }

            TEST(BitStream, Floats)
            {
                Common::U32 expectedStreamSize = sFloatCount * sizeof(Common::F32);

                Support::CBitStream floatStream(expectedStreamSize);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
                //EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                {
                    EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], floatStream.top<Common::F32>());
                    floatStream.pop<Common::F32>();
                }
            }

            TEST(BitStream, MemoryBlock)
            {
                Common::U32 expectedStreamSize = sFloatCount * sizeof(Common::F32);

                Support::CBitStream floatStream(expectedStreamSize);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
               // EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
               // Support::CBitStream blockStream(floatStream.raw(), floatStream.getSize());

              //  for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
               //{
                //   EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], blockStream.top<Common::F32>());
                 //   blockStream
               // }
            }

            TEST(BitStream, BadStartFloats)
            {
                Common::U32 expectedStreamSize = sFloatCount * sizeof(Common::F32);

                Support::CBitStream floatStream(expectedStreamSize - 3);
                PackFloats(floatStream);

                // Check if our BitStream size is correct
             //   EXPECT_EQ(expectedStreamSize, floatStream.getSize());

                // Now make sure we can unpack the data correctly
                for (Common::S32 iteration = sFloatCount - 1; iteration > -1; iteration--)
                {
                    EXPECT_EQ(sFloatList[iteration], floatStream.top<Common::F32>());
                    floatStream.pop<Common::F32>();
                }
            }

            TEST(BitStream, Vector)
            {
                Common::U32 expectedStreamSize = sFloatCount * sizeof(Common::F32);

                Common::Vector3DF testVector(1.0f, 2.0f, 3.0f);
                Support::CBitStream vectorStream(expectedStreamSize);
                vectorStream.write<Common::Vector3DF>(testVector);

               // const Common::Vector3DF &readVector = vectorStream.read<Common::Vector3DF>();

                // Are the components correct?
               // EXPECT_EQ(1.0f, readVector.X);
               // EXPECT_EQ(2.0f, readVector.Y);
               // EXPECT_EQ(3.0f, readVector.Z);

                // Kill it
                //delete &readVector;
            }

            TEST(BitStream, String)
            {
                // Write and read a singular String
                /*
                Support::CBitStream singleStringTest;

                singleStringTest.writeString(sStringList[0]);
                EXPECT_TRUE(singleStringTest.readString().compare(sStringList[0]) == 0);

                // Calculate the expected stream size
                Common::U32 expectedStreamSize = 0;

                for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                    expectedStreamSize += sStringList[iteration].length();

                // Pack them
                Support::CBitStream testStream;
                for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                    testStream.writeString(sStringList[iteration]);

                // Unpack
                for (Common::S32 iteration = sStringCount - 1; iteration > -1; iteration--)
                    EXPECT_TRUE(testStream.readString().compare(sStringList[iteration]) == 0);
                    */
            }
        #endif // _INCLUDE_KIARO_TESTS_H_
    } // End Namespace Support
} // End namespace Kiaro
