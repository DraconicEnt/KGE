/**
 *  @file TBitStream.cpp
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

            inline void PackFloats(Support::CBitStream& out)
            {
                for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                    out.write(sFloatList[iteration]);
            }

            inline void PackStrings(Support::CBitStream& out)
            {
                for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                    out.write(sStringList[iteration].data());
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
                    EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], *floatStream.top<Common::F32>());
                    floatStream.pop<Common::F32>();
                }
            }

            TEST(BitStream, MemoryBlock)
            {
                Common::U32 expectedStreamSize = sizeof(sFloatList);

                Support::CBitStream floatStream(expectedStreamSize);
                PackFloats(floatStream);

                // We wil have a memory block to use from the float stream
                Support::CBitStream blockStream(floatStream.getBlock(), floatStream.getWrittenLength());

                for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                {
                   EXPECT_EQ(sFloatList[(sFloatCount - 1) - iteration], *blockStream.top<Common::F32>());
                   blockStream.pop<Common::F32>();
                }
            }

            TEST(BitStream, BufferOverflow)
            {
                Common::U32 expectedStreamSize = sizeof(sFloatList);

                Support::CBitStream floatStream(expectedStreamSize - 3);

                bool caughtException = false;
                try
                {
                    PackFloats(floatStream);
                }
                catch (std::overflow_error& e)
                {
                    caughtException = true;
                }

                EXPECT_TRUE(caughtException);
            }

            TEST(BitStream, BufferUnderflow)
            {
                Common::U32 expectedStreamSize = sizeof(sFloatList);
                Support::CBitStream floatStream(expectedStreamSize);

                PackFloats(floatStream);
                bool caughtException = false;
                try
                {
                    floatStream.pop<Common::Vector3DF>();
                    floatStream.top<Common::Vector3DF>();
                }
                catch (std::underflow_error& e)
                {
                    caughtException = true;
                }

                EXPECT_TRUE(caughtException);
            }

            TEST(BitStream, Vector)
            {
                Common::U32 expectedStreamSize = 3 * sizeof(Common::F32);

                Common::Vector3DF testVector(1.0f, 2.0f, 3.0f);
                Support::CBitStream vectorStream(expectedStreamSize);
                vectorStream.write(testVector);

                const Common::Vector3DF* readVector = vectorStream.top<Common::Vector3DF>();

                // Are the components correct?
                EXPECT_EQ(1.0f, readVector->X);
                EXPECT_EQ(2.0f, readVector->Y);
                EXPECT_EQ(3.0f, readVector->Z);
            }

            TEST(BitStream, VectorToFloat)
            {
                Common::U32 expectedStreamSize = 3 * sizeof(Common::F32);

                Common::Vector3DF testVector(1.0f, 2.0f, 3.0f);
                Support::CBitStream vectorStream(expectedStreamSize);
                vectorStream.write(testVector);

                const Common::F32& readZ = *vectorStream.top<Common::F32>();
                vectorStream.pop<Common::F32>();
                const Common::F32& readY = *vectorStream.top<Common::F32>();
                vectorStream.pop<Common::F32>();
                const Common::F32& readX = *vectorStream.top<Common::F32>();
                vectorStream.pop<Common::F32>();

                // Are the components correct?
                EXPECT_EQ(1.0f, readX);
                EXPECT_EQ(2.0f, readY);
                EXPECT_EQ(3.0f, readZ);
            }

            TEST(BitStream, String)
            {
                Support::CBitStream stream(256);

                PackStrings(stream);

                size_t writtenBytes = 0;
                for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                        writtenBytes += sStringList[iteration].length() + sizeof(size_t) + 1;

                EXPECT_EQ(stream.getWrittenLength(), writtenBytes);

                for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                {
                    EXPECT_FALSE(strcmp(sStringList[(sStringCount - 1) - iteration].data(), stream.top<const Common::C8>()));
                    stream.pop<const Common::C8*>();
                }
            }
        #endif // _INCLUDE_KIARO_TESTS_H_
    } // End Namespace Support
} // End namespace Kiaro
