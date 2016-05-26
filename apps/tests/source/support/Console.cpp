/**
 *  @file CBitStream.cpp
 *  @brief Source file containing coding for the BitStream tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/UnorderedMap.hpp>
#include <support/Console.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Console
        {
            static Support::UnorderedMap<Common::U32, Common::U32> sLogCounts;

            static const Common::C8* sString = "This %u test";

            static void ConsoleListener(MESSAGE_TYPE type, const Support::String& message)
            {
                // Check the expected result
                Common::C8 buffer[256];
                sprintf(buffer, sString, sLogCounts[type]);

                EXPECT_FALSE(strcmp(buffer, message.data()));

                sLogCounts[type]++;
            }

            static void WriteLog(MESSAGE_TYPE type)
            {
                Console::writef(type, sString, sLogCounts[type]);
            }

            TEST(Console, Listener)
            {
                Console::registerListener(ConsoleListener, MESSAGE_INFO);
                Console::registerListener(ConsoleListener, MESSAGE_ERROR);

                for (Common::U32 iteration = 0; iteration < 10; iteration++)
                {
                    WriteLog(MESSAGE_INFO);

                    EXPECT_EQ(sLogCounts[MESSAGE_INFO], iteration + 1);
                    EXPECT_EQ(sLogCounts[MESSAGE_ERROR], 0);
                }

                sLogResponders.clear();
                sLogCounts.clear();
            }
        }
    } // End Namespace Support
} // End namespace Kiaro

