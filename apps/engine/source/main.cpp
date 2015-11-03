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

#include <support/Console.hpp>
#include <support/common.hpp>
#include <support/support.hpp>
#include <core/SEngineInstance.hpp>

#include <support/CCommandLineParser.hpp>
#include <support/CMapDivision.hpp>
#include <support/SSynchronousScheduler.hpp>

using namespace Kiaro;

/**
 *  @brief Standard entry point.
 *  @param arg A Kiaro::Common::S32 representing the total number of arguments passed to the program.
 *  @param argv An array of Kiaro::Common::C8 representing the parameters passed in to the program.
 *  @return A Kiaro::Common::S32 representing the exit code.
 */
Common::S32 main(Common::S32 argc, Common::C8 *argv[])
{
    Support::Console::write(Support::Console::MESSAGE_INFO, "------------------------------------------------");
    Support::Console::writef(Support::Console::MESSAGE_INFO, "Kiaro Game Engine %u.%u.%u", VERSION::MAJOR, VERSION::MINOR, VERSION::REVISION);
    Support::Console::write(Support::Console::MESSAGE_INFO, "------------------------------------------------");

    // Create our parser as a pointer so we can destroy it when entering the engine
    Kiaro::Support::CommandLineParser commandLineParser(argc, argv);

    // Register all of the descriptions
    commandLineParser.setFlagDescription("-h", "Displays this help text.");
    commandLineParser.setFlagDescription("-game", "<game name> : Start up the engine with a given game.");
    commandLineParser.setFlagDescription("-dedicated", "Start up the engine as a standalone server.");
    commandLineParser.setFlagDescription("-v", "Print versioning information.");
    commandLineParser.setFlagDescription("-addons", "<addon1> [addon2...] : Run the game with a list of addons installed.");

    // Should we display the help info?
    if (commandLineParser.hasFlag("-h"))
        commandLineParser.displayHelp(argc, argv);
    // Print the version information?
    else if (commandLineParser.hasFlag("-v"))
        Support::Console::writef(Support::Console::MESSAGE_INFO, "Engine Version %u.%u.%u", VERSION::MAJOR, VERSION::MINOR, VERSION::REVISION);

    // Run the thing?
    else if (commandLineParser.hasFlag("-game"))
    {
        const Support::Vector<Support::String> gameArguments = commandLineParser.getFlagArguments("-game");

        if (gameArguments.size() != 1)
        {
            Support::Console::write(Support::Console::MESSAGE_FATAL, "No game specified.\n");
            commandLineParser.displayHelp(argc, argv);

            // FIXME (Robert MacGregor#9): Segfault upon exit?
            return -1;
        }

        const Support::String gameName = gameArguments[0];

        // Get the addons list, if there is one
        Support::Vector<Support::String> addonList;
        if (commandLineParser.hasFlag("-addons") && commandLineParser.getFlagArgumentCount("-addons") == 0)
        {
            Support::Console::write(Support::Console::MESSAGE_FATAL, "No addons specified.\n");

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
                Support::Console::write(Support::Console::MESSAGE_FATAL, "No server specified.\n");

                commandLineParser.displayHelp(argc, argv);
                return -3;
            }

            engineMode = Core::MODE_CLIENTCONNECT;
        }

        // Create the Engine Instance
        Core::SEngineInstance* engineInstance = Core::SEngineInstance::getPointer();
        engineInstance->setMode(engineMode);
        engineInstance->setTargetServer((char*)targetServerIP.c_str(), 11595);
        engineInstance->setGame(gameName);
        engineInstance->start(argc, argv);

        // Release used memory when the start routine eventually returns
        Core::SEngineInstance::destroy();
    }

    Support::Console::write(Support::Console::MESSAGE_INFO, "EngineMain: Exited successfully.");

    return 0;
}
