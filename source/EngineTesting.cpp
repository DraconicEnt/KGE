/**
 *  @file Tests.cpp
 *  @brief Engine testing framework.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <EngineConfig.h>

#if ENGINE_TESTS>0
#include <tests/Tests.h>

#include <support/CommandLineParser.h>

#include <EngineLogging.h>

#include <boost/unordered_map.hpp>

#include <support/Array.hpp>
#include <support/String.h>

// List of tests
#include <tests/BitStream.hpp>

namespace Kiaro
{
    namespace Tests
    {
        static const EngineTestInformation TestList[] =
        {
            { "BitStream Test", "bitstream", Kiaro::Tests::BitStreamTest },
        };

        void PerformTests(Kiaro::Support::CommandLineParser *parser)
        {
            Kiaro::Common::U32 total_test_count = sizeof(TestList) / sizeof(EngineTestInformation);
            Kiaro::Logging::Write("Total Available Tests: %u", Kiaro::Logging::LEVEL_INFO, 1, total_test_count);

            // Make sure we actually requested any tests
            if (parser->GetFlagArgumentCount("-tests") == 0)
            {
                Kiaro::Logging::Write("No tests to run! Pass 'list' as the first test for a list of available tests.", Kiaro::Logging::LEVEL_ERROR);
                return;
            }

            // See if we just want a list of available tests
            if (strcmp(parser->GetFlagArgument("-tests", 0), "list") == 0)
            {
                Kiaro::Logging::Write("Available Tests: ");
                for (Kiaro::Common::U32 iteration = 0; iteration < total_test_count; iteration++)
                    Kiaro::Logging::Write("     * '%s'", Kiaro::Logging::LEVEL_INFO, 1, TestList[iteration].FlagName);

                return;
            }

            // Build a dictionary of flags test information
            boost::unordered_map<size_t, EngineTestInformation> test_flag_mappings;

            for (Kiaro::Common::U32 iteration = 0; iteration < total_test_count; iteration++)
                test_flag_mappings[Kiaro::Common::string_hash(TestList[iteration].FlagName)] = TestList[iteration];

            // Loop through everything passed to -tests
            Kiaro::Support::Array<EngineTestInformation> desired_tests = Kiaro::Support::Array<EngineTestInformation>();

            for (Kiaro::Common::U32 iteration = 0; iteration < parser->GetFlagArgumentCount("-tests"); iteration++)
            {
                Kiaro::Common::C8 *requested_test_name = parser->GetFlagArgument("-tests", iteration);
                size_t requested_test_name_hash = Kiaro::Common::string_hash(requested_test_name);

                try
                {
                    EngineTestInformation discovered_valid_test = test_flag_mappings.at(requested_test_name_hash);
                    desired_tests.Append(discovered_valid_test);
                }
                catch (std::out_of_range &exception)
                {
                    Kiaro::Logging::Write("Unknown test: %s", Kiaro::Logging::LEVEL_WARNING, 1, requested_test_name);
                    continue;
                }
            }

            // Did we actually get any tests?
            if (desired_tests.Len() == 0)
            {
                Kiaro::Logging::Write("No valid tests to execute! Pass 'list' as the first test for a list of available tests.", Kiaro::Logging::LEVEL_ERROR);
                return;
            }

            // Execute Tests
            Kiaro::Common::U32 test_exit_code = 0;
            Kiaro::Support::String passed_tests = "";
            Kiaro::Support::String failed_tests = "";
            Kiaro::Logging::Write("Performing Tests ----------------");
            for (Kiaro::Common::U32 iteration = 0; iteration < desired_tests.Len(); iteration++)
            {
                Kiaro::Logging::Write("Test Name: %s", Kiaro::Logging::LEVEL_INFO, 1, desired_tests[iteration].FullName);
                test_exit_code = desired_tests[iteration].Entry(parser);

                Kiaro::Logging::Write("Exit Code: %u -- %s", Kiaro::Logging::LEVEL_INFO, 2, test_exit_code, test_exit_code == 0 ? "Good":"Bad");

                if (test_exit_code == 0)
                {
                    passed_tests += desired_tests[iteration].FullName;
                    passed_tests += ", ";
                }
                else
                {
                    failed_tests += desired_tests[iteration].FullName;
                    failed_tests += ", ";
                }
            }
            Kiaro::Logging::Write("Tests Complete   ----------------");

            // Print Results
            Kiaro::Logging::Write("Tests Passed: %s", Kiaro::Logging::LEVEL_INFO, 1, passed_tests.C_Str());
            Kiaro::Logging::Write("Tests Failed: %s", Kiaro::Logging::LEVEL_INFO, 1, failed_tests.C_Str());
        }
    } // End NameSpace Tests
} // End NameSpace Kiaro
#endif // ENGINE_TESTS
