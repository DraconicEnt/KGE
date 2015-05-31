/**
 *  @file EngineMain.cpp
 *  @brief Main source code entry point.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <physfs.h>

#if ENGINE_TESTS>0
    #include <gtest/gtest.h>
#endif

#include <boost/iostreams/stream_buffer.hpp>
#include <boost/filesystem/fstream.hpp>

#include <core/common.hpp>
#include <support/support.hpp>
#include <core/SEngineInstance.hpp>

#include <support/CCommandLineParser.hpp>
#include <support/CMapDivision.hpp>
#include <core/SSynchronousScheduler.hpp>

using namespace Kiaro;

class DebugLog : public boost::iostreams::sink
{
    // Public Methods
    public:
        DebugLog(std::ostream &overwrittenStream, boost::filesystem::ofstream &logFileStream, const std::string textPrepend = "") : mOldStream(overwrittenStream),
        mTextPrepend(textPrepend), mWrotePrependText(false), mLogFileStream(logFileStream)
        {

        }

        std::streamsize write(const Kiaro::Common::C8 *str, std::streamsize size)
        {
            std::string newString(str, size);

            // Add the prepend text if we wrote a new line (and it's not just a newline char)
            if (!mTextPrepend.empty() && !mWrotePrependText && newString != "\n")
            {
                newString = mTextPrepend + newString;
                mWrotePrependText = true;
            }

            // Write to whatever old stream we have
            mOldStream.write(newString.c_str(), newString.length());
            mOldStream.flush();

            // Write to the log
            mLogFileStream.write(newString.c_str(), newString.length());
            mLogFileStream.flush();

            // If this contained a newline char, prepare the stream to write the prepend text next time around
            if (newString.rfind("\n") != std::string::npos)
                mWrotePrependText = false;

            return size;
        }

        // Private Members
        private:
            bool mWrotePrependText;
            boost::filesystem::ofstream &mLogFileStream;
            const std::string mTextPrepend;
            std::ostream &mOldStream;
};

/**
 *  @brief Standard entry point.
 *  @param arg A Kiaro::Common::S32 representing the total number of arguments passed to the program.
 *  @param argv An array of Kiaro::Common::C8 representing the parameters passed in to the program.
 *  @return A Kiaro::Common::S32 representing the exit code.
 */
Common::S32 main(Common::S32 argc, Common::C8 *argv[])
{
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Kiaro Game Engine" << std::endl;
    //std::cout << "Kiaro Game Engine v%u.%u.%u Build %u\n", KIARO_ENGINE_VERSION_MAJOR, KIARO_ENGINE_VERSION_MINOR, KIARO_ENGINE_VERSION_REVISION, KIARO_ENGINE_BUILD_COUNT);
    std::cout << "Copyright (c) 2014 Draconic Entertainment" << std::endl;
    std::cout <<"------------------------------------------------" << std::endl;

    // Create our parser as a pointer so we can destroy it when entering the engine
    Kiaro::Support::CommandLineParser commandLineParser(argc, argv);

    // Register all of the descriptions
    commandLineParser.setFlagDescription("-h", "Displays this help text.");
    commandLineParser.setFlagDescription("-game", "<game name> : Start up the engine with a given game.");
    commandLineParser.setFlagDescription("-dedicated", "Start up the engine as a standalone server.");
    commandLineParser.setFlagDescription("-v", "Print versioning information.");
    commandLineParser.setFlagDescription("-addons", "<addon1> [addon2...] : Run the game with a list of addons installed.");

    #if ENGINE_TESTS > 0
        ::testing::InitGoogleTest(&argc, argv);

        // Only add the -tests flag if we're actually building with the GTest unit tests.
        commandLineParser.setFlagDescription("-tests", "Tests the engine with the Google Testing framework.");

        if (commandLineParser.hasFlag("-tests"))
            RUN_ALL_TESTS();
    #endif // ENGINE_TESTS

    // Should we display the help info?
    if (commandLineParser.hasFlag("-h"))
        commandLineParser.displayHelp(argc, argv);
    // Print the version information?
    else if (commandLineParser.hasFlag("-v"))
    {
        std::cout << "Kiaro Game Engine version " << std::endl;
    }
    // Run the thing?
    else if (commandLineParser.hasFlag("-game"))
    {
        const Support::Vector<Support::String> gameArguments = commandLineParser.getFlagArguments("-game");

        if (gameArguments.size() != 1)
        {
            std::cerr << "No game specified." << std::endl << std::endl;

            commandLineParser.displayHelp(argc, argv);

            // FIXME (Robert MacGregor#9): Segfault upon exit?
            return -1;
        }

        const Support::String gameName = gameArguments[0];

        // Get the addons list, if there is one
        Support::Vector<Support::String> addonList;
        if (commandLineParser.hasFlag("-addons") && commandLineParser.getFlagArgumentCount("-addons") == 0)
        {
            std::cerr << "No addons specified." << std::endl << std::endl;

            commandLineParser.displayHelp(argc, argv);
            return -2;
        }
        else if (commandLineParser.hasFlag("-addons"))
            addonList = commandLineParser.getFlagArguments("-addons");

        Core::MODE_NAME engineMode = Core::MODE_CLIENT;

        if (commandLineParser.hasFlag("-dedicated"))
            engineMode = Core::MODE_DEDICATED;

        // Check for the -server <ip address> flag
        Support::String targetServerIP;
        if (commandLineParser.hasFlag("-server"))
        {
            targetServerIP = commandLineParser.getFlagArguments("-server")[0];
            if (targetServerIP.empty())
            {
                std::cerr << "No server specified with the server flag." << std::endl;

                commandLineParser.displayHelp(argc, argv);
                return -3;
            }

            engineMode = Core::MODE_CLIENTCONNECT;
        }

        // Create the Engine Instance
        Core::SEngineInstance *engineInstance = Core::SEngineInstance::getPointer();
        engineInstance->setMode(engineMode);
        engineInstance->setTargetServer((char*)targetServerIP.c_str(), 11595);
        engineInstance->setGame(gameName);
        engineInstance->start(argc, argv);

        // Release used memory when the start routine eventually returns
        Core::SEngineInstance::destroy();
    }

    std::cout << "EngineMain: Exited successfully." << std::endl;

    return 0;
}
