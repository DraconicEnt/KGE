/**
 *  @file CommandLineParser.cpp
 *  @brief Include file defining the CommandLineParser class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <regex> // std::regex, std::regex_match

#include <support/CCommandLineParser.hpp>

namespace Kiaro
{
    namespace Support
    {
        CommandLineParser::CommandLineParser(Kiaro::Common::S32 argc, Kiaro::Common::C8 **argv) : mCurrentLongestFlagLength(0)
        {
            mArgv = argv;
            mArgc = argc;

            static const std::regex flagRegex("-.+");

           	size_t currentFlagHash = 0;
			const Kiaro::Common::C8 *currentFlagName = NULL;

            for (size_t iteration = 1; iteration < argc; iteration++)
				if (std::regex_match(argv[iteration], flagRegex))
				{
					currentFlagName = argv[iteration];
					currentFlagHash = Kiaro::Common::string_hash(currentFlagName);

					mFlags.insert(std::make_pair(currentFlagHash, std::vector<Kiaro::Common::String>()));
				}
				else if (currentFlagName != NULL)
                    mFlags[currentFlagHash].push_back(argv[iteration]);

            // The command line parser implementation will always have an -h option to generate
            // a help from the information fed to it by the main source.
            FlagEntry *helpEntry = new FlagEntry;
            helpEntry->name = "-h";
            helpEntry->description = "Displays this help text.";
            helpEntry->responder = new FlagResponder::MemberDelegateType<CommandLineParser>(this, &CommandLineParser::displayHelp);

            setFlagResponder(helpEntry);
        }

        CommandLineParser::~CommandLineParser(void) { }

        bool CommandLineParser::hasFlag(const Kiaro::Common::C8 *flag)
        {
			size_t flagHash = Kiaro::Common::string_hash(flag);

			try
			{
				// NOTE (Robert MacGregor#1): Testing the existence of our flag by purposely raising an exception
				mFlags.at(flagHash);
				return true;
			}
			catch (const std::out_of_range &e)
			{
				return false;
			}

			return false;
        }

        Kiaro::Common::String CommandLineParser::getFlagArgument(const Kiaro::Common::C8 *targetFlag, Kiaro::Common::U8 argumentIdentifier)
        {
            if (!hasFlag(targetFlag))
                return "";

            size_t flagHash = Kiaro::Common::string_hash(targetFlag);

            std::vector<Kiaro::Common::String> &requestedFlagVector = mFlags[flagHash];
            if (argumentIdentifier >= requestedFlagVector.size())
                return "";

            return requestedFlagVector[argumentIdentifier];
        }

        size_t CommandLineParser::getFlagArgumentCount(const Kiaro::Common::C8 *targetFlag)
        {
            if (!hasFlag(targetFlag))
                return 0;

            size_t flagHash = Kiaro::Common::string_hash(targetFlag);
			return mFlags[flagHash].size();
        }

        void CommandLineParser::setFlagResponder(FlagEntry *entry)
        {
            size_t flagHash = Kiaro::Common::string_hash(entry->name);
            mFlagResponders[flagHash] = entry;
            mFlagEntries.push_back(entry);

            if (entry->name.length() > mCurrentLongestFlagLength)
                mCurrentLongestFlagLength = entry->name.length() + 5;
        }

        void CommandLineParser::invokeFlagResponders(void)
        {
            bool executedFlagHandler = false;

            for (std::unordered_map<size_t, std::vector<Kiaro::Common::String>>::iterator it = mFlags.begin(); it != mFlags.end(); it++)
                try
                {
                    size_t currentFlagHash = (*it).first;

                    FlagResponder *currentResponder = mFlagResponders.at(currentFlagHash)->responder;

                    // Don't try to execute the responder if there isn't one
                    if (!currentResponder)
                        continue;

                    currentResponder->invoke(this, mArgc, mArgv, mFlags[currentFlagHash], executedFlagHandler);
                    executedFlagHandler = true;
                }
                catch (std::out_of_range &e) { }

            if (!executedFlagHandler)
            {
                size_t helpHash = Kiaro::Common::string_hash("-h");
                std::cout << "No command line arguments provided. " << std::endl << std::endl;
                displayHelp(this, mArgc, mArgv, mFlags[helpHash], false);
            }
        }

        void CommandLineParser::displayHelp(CommandLineParser *parser, const Kiaro::Common::S32 &argc, Kiaro::Common::C8 *argv[], const std::vector<Kiaro::Common::String> &arguments, bool otherFlags)
        {
            std::cout << "You may run " << argv[0] << " with:" << std::endl;

            for (std::vector<FlagEntry *>::iterator it = mFlagEntries.begin(); it != mFlagEntries.end(); it++)
            {
                FlagEntry *currentEntry = (*it);

                // Output the help
                std::cout << "  " << currentEntry->name;

                for (size_t iteration = 0; iteration < mCurrentLongestFlagLength - currentEntry->name.length(); iteration++)
                    std::cout << " ";

                std::cout << currentEntry->description << std::endl;
            }
        }
    } // End Namespace Support
} // End Namespace Kiaro
