/**
 *  @file file.cpp
 *  @brief Source file containing coding for the file tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <cstdio>

#include <gtest/gtest.h>

#include <support/common.hpp>
#include <support/platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        static const Common::F32 Pi = 3.14f;
        static const Common::U32 Uber = 1337;

        TEST(Platform, FileMemoryMapRead)
        {
            // Ensure our file exists
            FILE* handle = fopen("existingFile.dat", "w");
            EXPECT_TRUE(handle != nullptr);

            // We can't continue the test if we didn't even get a valid handle
            if (!handle)
                return;

            fwrite(&Pi, sizeof(Common::F32), 1, handle);
            fwrite(&Uber, sizeof(Common::U32), 1, handle);

            fclose(handle);
            File::MemoryMappedFile file("existingFile.dat", File::ACCESS_MODE::READ);

            file.open();
            EXPECT_TRUE(file.mAddress != nullptr);
            EXPECT_EQ(sizeof(Common::F32) + sizeof(Common::S32), file.mLength);

            // If we're at this point, we should be able to read off Pi and then Uber in the memory
            Common::F32& readFloat = *reinterpret_cast<Common::F32*>(file.mAddress);
            Common::U32& readInt = *(reinterpret_cast<Common::U32*>(file.mAddress) + 1);

            EXPECT_EQ(Pi, readFloat);
            EXPECT_EQ(Uber, readInt);
        }

        TEST(Platform, FileMemoryMapWriteNew)
        {
            File::MemoryMappedFile file("writeNew.dat", File::ACCESS_MODE::WRITE);
            file.mLength = 16;
            file.open();

            EXPECT_TRUE(file.mAddress != nullptr);
            Common::F32& someFloat = *reinterpret_cast<Common::F32*>(file.mAddress);
            someFloat = 15;

            EXPECT_EQ(15, someFloat);
            file.close();
            file.open();

            someFloat = *reinterpret_cast<Common::F32*>(file.mAddress);

            EXPECT_EQ(15, someFloat);
        }
    } // End Namespace Support
} // End namespace Kiaro
