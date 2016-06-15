/**
 *  @file CCommandLineParser.cpp
 */

#include <gtest/gtest.h>

#include <support/CCommandLineParser.hpp>

namespace Kiaro
{
    namespace Support
    {
        static const Common::C8* sArgv[] =
        {
            "One",
            "-Two",
            "-Three",
            "Four",
            "Five",
            "Six",
            "-Seven",
        };

        static Common::S32 sArgc = sizeof(sArgv) / sizeof(const Common::C8*);

        TEST(CCommandLineParser, FlagCheck)
        {
            CommandLineParser commandLineParser(sArgc, sArgv);
            EXPECT_FALSE(commandLineParser.hasFlag("One"));
            EXPECT_FALSE(commandLineParser.hasFlag("-One"));
            EXPECT_TRUE(commandLineParser.hasFlag("-Two"));
            EXPECT_FALSE(commandLineParser.hasFlag("-two"));
        }

        TEST(CCommandLineParser, FlagArguments)
        {
            CommandLineParser commandLineParser(sArgc, sArgv);
            Vector<String> currentArguments = commandLineParser.getFlagArguments("One");
            EXPECT_EQ(0, currentArguments.size());
            currentArguments = commandLineParser.getFlagArguments("-One");
            EXPECT_EQ(0, currentArguments.size());
            currentArguments = commandLineParser.getFlagArguments("-Three");
            EXPECT_EQ(3, currentArguments.size());

            for (Common::U32 iteration = 0; iteration < currentArguments.size(); iteration++)
                EXPECT_EQ(sArgv[iteration + 3], currentArguments[iteration]);

            currentArguments = commandLineParser.getFlagArguments("-Seven");
            EXPECT_EQ(0, currentArguments.size());
        }
    } // End Namespace Support
} // End namespace Kiaro
