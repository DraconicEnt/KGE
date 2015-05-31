/**
 *  @file CommandLineParser.h
 *  @brief Include file defining the Kiaro::Suport::CommandLineParser class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
#define _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_

#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map

#include <easydelegate.hpp>

#include <support/support.hpp>
#include <core/common.hpp>
#include <core/config.hpp>

namespace Kiaro
{
    namespace Support
    {
        /** Class that supplies a robust command line parser.
         *
         *  The CommandLineParser class parses console input where any arguments prepended with
         *  a dash is considered a flag while the rest is considered arguments to the flags.
         *
         *  <b>Ex:</b> ./kiaro64_d -game base -server 98.238.10.100 -port 11595
         */
        class CommandLineParser
        {
            // Public Methods
            public:
                typedef EasyDelegate::DelegateBase<void, CommandLineParser *, const Common::S32 &, Common::C8 **, const std::vector<Support::String>&, bool> FlagResponder;

                typedef struct
                {
                    Support::String name;
                    Support::String description;
                    FlagResponder *responder;
                } FlagEntry;

                /**
                 *  @brief Constructor accepting the argument count and an argument array.
                 *  @param argc A Kiaro::s32 representing the total number of arguments in our array.
                 *  @param argv An array of Kiaro::c8 representing the input arguments to parse.
                 */
                CommandLineParser(Common::S32 argc, Common::C8 **argv);

                //! Standard Destructor
                ~CommandLineParser(void);

                /**
                 *  @brief Returns whether or not the CommandLineParser has a given flag.
                 *  @param flag A Kiaro::String representing the flag name to test for.
                 *  @return A boolean representing whether or not the CommandLineParser had the flag.
                 */
                bool hasFlag(const Common::C8 *flagName);

                Support::Vector<Support::String> getFlagArguments(const Common::C8 *targetFlag);

                size_t getFlagArgumentCount(const Common::C8 *targetFlag);

                void setFlagDescription(const Support::String &flagName, const Support::String &description);

                void displayHelp(const Common::S32 &argc, Common::C8 *argv[]);

            // Private Members
            private:
                Common::C8 **mArgv;
                Common::S32 mArgc;

                Support::UnorderedMap<size_t, Support::Vector<Support::String>> mFlags;
                Support::UnorderedMap<size_t, Support::Pair<Support::String, Support::String>> mFlagDescriptions;

                Support::Vector<FlagEntry *> mFlagEntries;

                Support::String mCommandLine;

                size_t mCurrentLongestFlagLength;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
