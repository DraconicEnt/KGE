/**
 *  @file CBitStream.cpp
 *  @brief Source file containing coding for the BitStream tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <gtest/gtest.h>

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
                out.write(sFloatList[iteration]);
        }

        inline void PackStrings(Support::CBitStream& out)
        {
            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                out.writeString(sStringList[iteration].data());
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
                EXPECT_EQ(sFloatList[iteration], floatStream.pop<Common::F32>());
        }

        TEST(BitStream, MemoryBlock)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);

            CBitStream floatStream(expectedStreamSize);
            PackFloats(floatStream);

            // We wil have a memory block to use from the float stream
            CBitStream blockStream(floatStream.getBlock(), floatStream.getPointer());
            
            for (Common::U32 iteration = 0; iteration < sFloatCount; iteration++)
                EXPECT_EQ(sFloatList[iteration], blockStream.pop<Common::F32>());
        }

        TEST(BitStream, BufferOverflow)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);

            CBitStream floatStream(expectedStreamSize - 3);

            bool caughtException = false;
            EXPECT_THROW(PackFloats(floatStream), std::overflow_error);
        }

        TEST(BitStream, BufferUnderflow)
        {
            Common::U32 expectedStreamSize = sizeof(sFloatList);
            CBitStream floatStream(expectedStreamSize);

            PackFloats(floatStream);
            EXPECT_THROW(floatStream.pop<Common::Vector3DF>(), std::underflow_error);
            EXPECT_THROW(floatStream.top<Common::Vector3DF>(), std::underflow_error);
        }

        TEST(BitStream, String)
        {
            CBitStream stream(256);

            PackStrings(stream);

            size_t writtenBytes = 0;
            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                writtenBytes += sStringList[iteration].length() + sizeof(size_t) + 1;

            EXPECT_EQ(stream.getPointer(), writtenBytes);
            stream.setPointer(0);

            for (Common::U32 iteration = 0; iteration < sStringCount; iteration++)
                EXPECT_FALSE(strcmp(sStringList[iteration].data(), stream.popString()));
        }
        
        TEST(BitStream, InvalidString)
        {
            CBitStream stream(256);
            
            char payload[8];
            for (Common::U32 iteration = 0; iteration < 8; iteration++)
                payload[iteration] = 0x52;
                
            stream.writeString(payload);
            
            EXPECT_THROW(stream.popString(), std::runtime_error);
        }
    } // End Namespace Support
} // End namespace Kiaro
