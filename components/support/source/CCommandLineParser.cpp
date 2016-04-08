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

#include <support/CCommandLineParser.hpp>

namespace Kiaro
{
    namespace Support
    {
        CommandLineParser::CommandLineParser(Common::S32 argc, Common::C8** argv) : mArgv(argv), mArgc(argc),
        mCurrentLongestFlagLength(0)
        {
           	size_t currentFlagHash = 0;
			const Common::C8* currentFlagName = NULL;

            for (size_t iteration = 1; iteration < argc; iteration++)
				if (argv[iteration][0] == '-')
				{
					currentFlagName = argv[iteration];
					currentFlagHash = Support::getHashCode(currentFlagName);

					mFlags.insert(std::make_pair(currentFlagHash, Support::Vector<Support::String>()));
				}
				else if (currentFlagName != NULL)
                    mFlags[currentFlagHash].push_back(argv[iteration]);

        }

        CommandLineParser::~CommandLineParser(void) { }

        bool CommandLineParser::hasFlag(const Common::C8* flag) const
        {
			size_t flagHash = Support::getHashCode(flag);
			return mFlags.find(flagHash) != mFlags.end();
        }

        Support::Vector<Support::String> CommandLineParser::getFlagArguments(const Common::C8* targetFlag)
        {
            if (!this->hasFlag(targetFlag))
                return Support::Vector<Support::String>();

            size_t flagHash = Support::getHashCode(targetFlag);
            return mFlags[flagHash];
        }

        size_t CommandLineParser::getFlagArgumentCount(const Common::C8* targetFlag)
        {
            if (!hasFlag(targetFlag))
                return 0;

            size_t flagHash = Support::getHashCode(targetFlag);
			return mFlags[flagHash].size();
        }

        void CommandLineParser::setFlagDescription(const Support::String& flagName, const Support::String& description)
        {
            if (flagName.length() >= mCurrentLongestFlagLength)
                mCurrentLongestFlagLength = flagName.length();

            const size_t flagNameHash = Support::getHashCode(flagName);
            mFlagDescriptions.insert({flagNameHash, {flagName, description}});
        }

        void CommandLineParser::displayHelp(const Common::S32& argc, Common::C8* argv[])
        {
            std::cout << "You may run " << argv[0] << " with:" << std::endl;

            for (auto it = mFlagDescriptions.begin(); it != mFlagDescriptions.end(); it++)
            {
                Support::Pair<Support::String, Support::String> currentEntry = (*it).second;

                std::cout << "  " << currentEntry.first;

                for (size_t iteration = 0; iteration < (mCurrentLongestFlagLength + 1) - currentEntry.first.length(); iteration++)
                    std::cout << " ";

                std::cout << currentEntry.second << std::endl;
            }
        }
    } // End Namespace Support
} // End Namespace Kiaro
