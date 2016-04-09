/**
 *  @file CommandLineParser.hpp
 *  @brief Include file defining the Suport::CommandLineParser class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
#define _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_

#include <support/String.hpp>
#include <support/Vector.hpp>
#include <support/UnorderedMap.hpp> // std::unordered_map

#include <support/types.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief Class that supplies a robust command line parser.
         *
         *  @detail The CommandLineParser class parses console input where any arguments prepended with
         *  a dash is considered a flag while the rest is considered arguments to the flags.
         *
         *  <b>Ex:</b> ./kiaro64_d -game base -server 98.238.10.100 -port 11595
         */
        class CommandLineParser
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting the argument count and an argument array.
                 *  @param argc A Kiaro::s32 representing the total number of arguments in our array.
                 *  @param argv An array of Kiaro::c8 representing the input arguments to parse.
                 */
                CommandLineParser(Common::S32 argc, Common::C8** argv);

                //! Standard Destructor
                ~CommandLineParser(void);

                /**
                 *  @brief Returns whether or not the CommandLineParser has a given flag.
                 *  @param flag A Kiaro::String representing the flag name to test for.
                 *  @return A boolean representing whether or not the CommandLineParser had the flag.
                 */
                bool hasFlag(const Common::C8* flagName) const;

                /**
                 *  @brief Returns a Support::Vector containg all arguments for the given flag name.
                 *  @param targetFlag The name of the flag to look up. These are command-line parameters
                 *  that are marked with a - preceding them.
                 *  @return A Support::Vector of all non-flag values following the flag.
                 */
                Support::Vector<Support::String> getFlagArguments(const Common::C8* targetFlag);

                size_t getFlagArgumentCount(const Common::C8* targetFlag);

                void setFlagDescription(const Support::String& flagName, const Support::String& description);

                /**
                 *  @brief Prints out help for the command line system to the console.
                 *  @param argc The argc passed in to the program via the main method.
                 *  @param argv The argv passed in to the program via the main method.
                 */
                void displayHelp(const Common::S32& argc, Common::C8* argv[]);

            // Private Members
            private:
                //! The argv passed into the program's main method.
                Common::C8** mArgv;
                //! The argc passed into the program's main method.
                Common::S32 mArgc;

                //! A mapping of flag name hashes to a Support::Vector containing all non-flag values immediately following in the command line input.
                Support::UnorderedMap<size_t, Support::Vector<Support::String>> mFlags;
                //! A mapping of flag name hashes to a Support::Pair containing the flag name and a short description of its functionality.
                Support::UnorderedMap<size_t, Support::Pair<Support::String, Support::String>> mFlagDescriptions;

                //! The full command line input as a single string, unmolested.
                Support::String mCommandLine;

                //! The length of the longest command line flag, this is used for outputting the help text in a neatly formatted fashion.
                size_t mCurrentLongestFlagLength;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
