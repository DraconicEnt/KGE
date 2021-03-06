/**
 *  @file CBitStream.cpp
 *  @brief Source file containing coding for the BitStream tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2021 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/types.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
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
            {
                out.write(sFloatList[iteration]);
            }
        }

        inline void PackStrings(Support::CBitStream& out)
        {
            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
            {
                out.writeString(sStringList[iteration].data());
            }
        }

        TEST(BitStream, Floats)
        {
            Common::U32 expectedStreamSize = sFloatCount * sizeof(Common::F32);
            CBitStream floatStream(expectedStreamSize);
            PackFloats(floatStream);
            floatStream.setPointer(0);

            // Check if our BitStream size is correct
            //EXPECT_EQ(expectedStreamSize, floatStream.getSize());

            // Now make sure we can unpack the data correctly
            for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
            {
                EXPECT_EQ(sFloatList[iteration], floatStream.pop<Common::F32>());
            }
        }

        TEST(BitStream, MemoryBlock)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);
            CBitStream floatStream(expectedStreamSize);
            PackFloats(floatStream);

            // We wil have a memory block to use from the float stream
            CBitStream blockStream(floatStream.getBlock(), floatStream.getPointer());

            for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
            {
                EXPECT_EQ(sFloatList[iteration], blockStream.pop<Common::F32>());
            }
        }

        TEST(BitStream, BufferOverflow)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);
            CBitStream floatStream(expectedStreamSize - 3);

            EXPECT_THROW(PackFloats(floatStream), std::overflow_error);
        }

        TEST(BitStream, BufferUnderflow)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);
            CBitStream floatStream(expectedStreamSize);
            PackFloats(floatStream);

            EXPECT_THROW(floatStream.pop<Support::Vector3DF>(), std::underflow_error);
            EXPECT_THROW(floatStream.top<Support::Vector3DF>(), std::underflow_error);
        }

        TEST(BitStream, String)
        {
            CBitStream stream(256);
            PackStrings(stream);
            size_t writtenBytes = 0;

            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
            {
                writtenBytes += sStringList[iteration].length() + sizeof(Common::U32) + 1;
            }

            EXPECT_EQ(stream.getPointer(), writtenBytes);
            stream.setPointer(0);

            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
            {
                EXPECT_FALSE(strcmp(sStringList[iteration].data(), stream.popString()));
            }
        }

        TEST(BitStream, InvalidString)
        {
            CBitStream stream(256);
            char payload[8];
            memset(payload, 0x58, 8);
            payload[5] = 0x00;

            // String isn't properly NULL terminated to be of this length
            EXPECT_THROW(stream.writeString(payload, 8), std::runtime_error);
            EXPECT_NO_THROW(stream.writeString(payload, 5));
            EXPECT_EQ(6 + sizeof(Common::U32), stream.getPointer());
        }

        TEST(BitStream, LongString)
        {
            CBitStream stream(8);
            const char* payload = "This string is too long to fit into our bit stream memory.";
            EXPECT_THROW(stream.writeString(payload), std::runtime_error);
        }

        TEST(BitStream, DynamicResize)
        {
            CBitStream stream(8, nullptr, 0, 32);

            // We should be able to pack the floats
            EXPECT_NO_THROW(PackFloats(stream));

            EXPECT_TRUE(stream.isFull());
            EXPECT_EQ(8, stream.getSize());

            // Now if we pack floats again, we should resize to 32+8 bytes
            EXPECT_NO_THROW(PackFloats(stream));

            EXPECT_FALSE(stream.isFull());
            EXPECT_EQ(40, stream.getSize());

            // But the pointer should only be at 8+8 bytes
            EXPECT_EQ(16, stream.getPointer());

            // Now make sure we can unpack the data correctly
            stream.setPointer(0);

            for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
            {
                EXPECT_EQ(sFloatList[iteration], stream.pop<Common::F32>());
            }
        }

        TEST(BitStream, EndianSwapping)
        {
            CBitStream stream(32);
            stream.mInverseEndian = true;

            Common::F32 sourceFloatVal = 3.14f;
            Common::U32 sourceIntVal = 1337;
            Common::U64 sourceLongVal = 69;
            Common::U16 sourceShortVal = 11595;

            stream << sourceFloatVal << sourceIntVal << sourceLongVal << sourceShortVal;
            stream.setPointer(0);

            // Ensure the source values weren't swapped
            EXPECT_EQ(3.14f, sourceFloatVal);
            EXPECT_EQ(1337, sourceIntVal);
            EXPECT_EQ(69, sourceLongVal);
            EXPECT_EQ(11595, sourceShortVal);

            // Read out new values and ensure they're not equal (because the values should be different)
            Common::F32 outputFloatVal = 3.14f;
            Common::U32 outputIntVal = 1337;
            Common::U64 outputLongVal = 69;
            Common::U16 outputShortVal = 11595;

            stream >> outputFloatVal >> outputIntVal >> outputLongVal >> outputShortVal;

            EXPECT_NE(sourceFloatVal, outputFloatVal);
            EXPECT_NE(sourceIntVal, outputIntVal);
            EXPECT_NE(sourceLongVal, outputLongVal);
            EXPECT_NE(sourceShortVal, outputShortVal);

            // Now test the endianness of our values
            SwapEndian(sourceFloatVal);
            SwapEndian(sourceIntVal);
            SwapEndian(sourceLongVal);
            SwapEndian(sourceShortVal);

            EXPECT_EQ(sourceFloatVal, outputFloatVal);
            EXPECT_EQ(sourceIntVal, outputIntVal);
            EXPECT_EQ(sourceLongVal, outputLongVal);
            EXPECT_EQ(sourceShortVal, outputShortVal);
        }
    } // End Namespace Support
} // End namespace Kiaro
