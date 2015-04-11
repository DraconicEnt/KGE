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

#include <engine/common.hpp>
#include <engine/config.hpp>

namespace Kiaro
{
    namespace Support
    {
        /** Class that supplies a robust command line parser.
         *
         *  The CommandLineParser class parses console input where any arguments prepended with
         *  a dash is considered a flag while the rest is considered arguments to the flags.
         *
         *  <b>Ex:</b> ./kiaro64_d -Game base -Server 98.238.10.100 -Port 11595
         */
        class CommandLineParser
        {
            // Public Methods
            public:
                typedef EasyDelegate::DelegateBase<void, CommandLineParser *, const Kiaro::Common::S32 &, Kiaro::Common::C8 **, const std::vector<Kiaro::Common::String>&, bool> FlagResponder;

                typedef struct
                {
                    std::string name;
                    std::string description;
                    FlagResponder *responder;
                } FlagEntry;

                /**
                 *  @brief Constructor accepting the argument count and an argument array.
                 *  @param argc A Kiaro::s32 representing the total number of arguments in our array.
                 *  @param argv An array of Kiaro::c8 representing the input arguments to parse.
                 */
                CommandLineParser(Kiaro::Common::S32 argc, Kiaro::Common::C8 **argv);

                //! Standard Destructor
                ~CommandLineParser(void);

                /**
                 *  @brief Returns whether or not the CommandLineParser has a given flag.
                 *  @param flag A Kiaro::String representing the flag name to test for.
                 *  @return A boolean representing whether or not the CommandLineParser had the flag.
                 */
                bool hasFlag(const Kiaro::Common::C8 *flagName);

                Kiaro::Common::String getFlagArgument(const Kiaro::Common::C8 *targetFlag, Kiaro::Common::U8 argumentIdentifier);

                size_t getFlagArgumentCount(const Kiaro::Common::C8 *targetFlag);

                void setFlagResponder(FlagEntry *entry);
                void invokeFlagResponders(void);

                void displayHelp(CommandLineParser *parser, const Kiaro::Common::S32 &argc, Kiaro::Common::C8 *argv[], const std::vector<Kiaro::Common::String> &arguments, bool otherFlags);

            // Private Members
            private:
                Kiaro::Common::C8 **mArgv;
                Kiaro::Common::S32 mArgc;

                std::unordered_map<size_t, std::vector<Kiaro::Common::String>> mFlags;
                std::unordered_map<size_t, FlagEntry *> mFlagResponders;

                std::vector<FlagEntry *> mFlagEntries;

                std::string mCommandLine;

                size_t mCurrentLongestFlagLength;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
