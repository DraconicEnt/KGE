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

#include <iostream>

#include <physfs.h>

#if ENGINE_TESTS>0
#include <gtest/gtest.h>
#endif

#include <boost/iostreams/stream_buffer.hpp>
#include <boost/filesystem/fstream.hpp>

#include <engine/Logging.hpp>

#include <engine/common.hpp>
#include <engine/SEngineInstance.hpp>

#include <support/CCommandLineParser.hpp>
#include <support/CMapDivision.hpp>
#include <engine/SScheduler.hpp>

static void gameFlagHandler(Kiaro::Support::CommandLineParser *parser, const Kiaro::Common::S32 &argc, Kiaro::Common::C8 *argv[], const std::vector<std::string> &arguments, bool otherFlags)
{
    if (otherFlags)
        return;

    if (arguments.size() != 1)
    {
        std::cerr << "No game specified." << std::endl << std::endl;

        parser->displayHelp(parser, argc, argv, arguments, otherFlags);
        return;
    }

    // Get the addon list or report an error
    std::vector<std::string> addonList;
    if (parser->hasFlag("-addons") && parser->getFlagArgumentCount("-addons") == 0)
    {
        std::cerr << "No addons specified." << std::endl << std::endl;

        parser->displayHelp(parser, argc, argv, arguments, otherFlags);
        return;
    }
    else if (parser->hasFlag("-addons"))
        for (Kiaro::Common::U32 iteration = 0; iteration < parser->getFlagArgumentCount("-addons"); iteration++)
            addonList.push_back(parser->getFlagArgument("-addons", iteration));

    Kiaro::Engine::MODE_NAME engineMode = Kiaro::Engine::MODE_CLIENT;

    if (parser->hasFlag("-dedicated"))
        engineMode = Kiaro::Engine::MODE_DEDICATED;

    // Check for the -server <ip address> flag
    std::string targetServerIP;
    if (parser->hasFlag("-server"))
    {
		targetServerIP = parser->getFlagArgument("-server", 0);
        if (targetServerIP.empty())
        {
            std::cerr << "No server specified with the server flag." << std::endl;

            parser->displayHelp(parser, argc, argv, arguments, otherFlags);
            return;
        }

        engineMode = Kiaro::Engine::MODE_CLIENTCONNECT;
    }

    // Create the Engine Instance
    Kiaro::Engine::SEngineInstance *engineInstance = Kiaro::Engine::SEngineInstance::getPointer();
    engineInstance->setMode(engineMode);
    engineInstance->setTargetServer((char*)targetServerIP.c_str(), 11595);
    engineInstance->setGame(arguments[0]);
    engineInstance->start(argc, argv);

    // Release used memory when the start routine eventually returns
    Kiaro::Engine::SEngineInstance::destroy();
}

#if ENGINE_TESTS>0
static void testsFlagHandler(Kiaro::Support::CommandLineParser *parser, const Kiaro::Common::S32 &argc, Kiaro::Common::C8 *argv[], const std::vector<std::string> &arguments, bool otherFlags)
{
    if (otherFlags)
        return;

    RUN_ALL_TESTS();
}
#endif // ENGINE_TESTS

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

static void versionFlagHandler(Kiaro::Support::CommandLineParser *parser, const Kiaro::Common::S32 &argc, Kiaro::Common::C8 *argv[], const std::vector<std::string> &arguments, bool otherFlags)
{
    std::cout << "Kiaro Game Engine version " << std::endl;
}

/**
 *  @brief Standard entry point.
 *  @param arg A Kiaro::Common::S32 representing the total number of arguments passed to the program.
 *  @param argv An array of Kiaro::Common::C8 representing the parameters passed in to the program.
 *  @return A Kiaro::Common::S32 representing the exit code.
 */
Kiaro::Common::S32 main(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[])
{
    // Init PhysFS
    PHYSFS_init(argv[0]);
    PHYSFS_setSaneConfig("Draconic Entertainment", "KGE", "ZIP", 0, 0);

    // Make sure the log file exists
    PHYSFS_File *file = PHYSFS_openWrite("LOG.txt");
    PHYSFS_write(file, " ", 1, 1);
    PHYSFS_close(file);

    // Produce the absolute file path to our logfile
    std::string logFilePath = PHYSFS_getRealDir("LOG.txt");
    logFilePath += "/LOG.txt";

    // Create the stream to the log file
    boost::filesystem::ofstream logFileStream;
    logFileStream.open(boost::filesystem::path(logFilePath));

    // Perform cout and cerr redirection
    boost::iostreams::stream_buffer<DebugLog> newStdoutStreamBuffer;
    std::streambuf *oldStdoutBuff = std::cout.rdbuf(&newStdoutStreamBuffer);
    std::ostream oldStdoutStream(oldStdoutBuff);
    newStdoutStreamBuffer.open(DebugLog(oldStdoutStream, logFileStream));

    boost::iostreams::stream_buffer<DebugLog> newStderrStreamBuffer;
    std::streambuf *oldStderrBuff = std::cerr.rdbuf(&newStderrStreamBuffer);
    std::ostream oldStderrStream(oldStderrBuff);
    newStderrStreamBuffer.open(DebugLog(oldStderrStream, logFileStream, "Error: "));

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Kiaro Game Engine" << std::endl;
    //std::cout << "Kiaro Game Engine v%u.%u.%u Build %u\n", KIARO_ENGINE_VERSION_MAJOR, KIARO_ENGINE_VERSION_MINOR, KIARO_ENGINE_VERSION_REVISION, KIARO_ENGINE_BUILD_COUNT);
    std::cout << "Copyright (c) 2014 Draconic Entertainment" << std::endl;
    std::cout <<"------------------------------------------------" << std::endl;

    Kiaro::Support::CommandLineParser commandLineParser(argc, argv);

    Kiaro::Support::CommandLineParser::FlagEntry *currentFlagEntry = new Kiaro::Support::CommandLineParser::FlagEntry;
    currentFlagEntry->name = "-game";
    currentFlagEntry->description = "<game name> : Start up the engine with a given game.";
    currentFlagEntry->responder = new Kiaro::Support::CommandLineParser::FlagResponder::StaticDelegateType(gameFlagHandler);
    commandLineParser.setFlagResponder(currentFlagEntry);

    currentFlagEntry = new Kiaro::Support::CommandLineParser::FlagEntry;
    currentFlagEntry->name = "-dedicated";
    currentFlagEntry->description = "Start up the engine as a standalone server.";
    currentFlagEntry->responder = NULL; // No Responder
    commandLineParser.setFlagResponder(currentFlagEntry);

    currentFlagEntry = new Kiaro::Support::CommandLineParser::FlagEntry;
    currentFlagEntry->name = "-v";
    currentFlagEntry->description = "Print versioning information.";
    currentFlagEntry->responder = new Kiaro::Support::CommandLineParser::FlagResponder::StaticDelegateType(versionFlagHandler);
    commandLineParser.setFlagResponder(currentFlagEntry);

    currentFlagEntry = new Kiaro::Support::CommandLineParser::FlagEntry;
    currentFlagEntry->name = "-addons";
    currentFlagEntry->description = "<addon1> [addon2...] : Run the game with a list of addons installed.";
    currentFlagEntry->responder = NULL;
    commandLineParser.setFlagResponder(currentFlagEntry);

    // Add a -tests response flag
    #if ENGINE_TESTS>0
    ::testing::InitGoogleTest(&argc, argv);

    currentFlagEntry = new Kiaro::Support::CommandLineParser::FlagEntry;
    currentFlagEntry->name = "-tests";
    currentFlagEntry->description = "Tests the engine with the Google Testing framework.";
    currentFlagEntry->responder = new Kiaro::Support::CommandLineParser::FlagResponder::StaticDelegateType(testsFlagHandler);
    commandLineParser.setFlagResponder(currentFlagEntry);
    #endif // ENGINE_TESTS

    commandLineParser.invokeFlagResponders();

    std::cout << "EngineMain: Exited successfully." << std::endl;

    // Cleanup and restore our streams
    logFileStream.close();
    std::cout.rdbuf(oldStdoutBuff);
    std::cerr.rdbuf(oldStderrBuff);

    return 0;
}
