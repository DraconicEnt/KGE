/**
 *  @file EngineMain.cpp
 *  @brief Main source code entry point.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2014 Draconic Entity
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
    CONSOLE_INFO("------------------------------------------------");
    CONSOLE_INFOF("Kiaro Game Engine %u.%u.%u", VERSION::MAJOR, VERSION::MINOR, VERSION::REVISION);
    CONSOLE_INFO("------------------------------------------------");

    // Create our parser as a pointer so we can destroy it when entering the engine
    Kiaro::Support::CommandLineParser commandLineParser(argc, argv);

    // Register all of the descriptions
    commandLineParser.setFlagDescription("-h", "Displays this help text.");
    commandLineParser.setFlagDescription("-game", "<game name> : Start up the engine with a given game.");
    commandLineParser.setFlagDescription("-mods", "<mod 1> [mod 2...] : A list of mods to run with the specified game.");
    commandLineParser.setFlagDescription("-dedicated", "Start up the engine as a standalone server.");
	commandLineParser.setFlagDescription("-perfstat", "Occasionally write performance statistics to stdout.");
    commandLineParser.setFlagDescription("-v", "Print versioning information.");

    // Should we display the help info?
    if (commandLineParser.hasFlag("-h"))
        commandLineParser.displayHelp(argc, argv);
    else if (commandLineParser.hasFlag("-v"))
        CONSOLE_INFOF("Engine Version %u.%u.%u", VERSION::MAJOR, VERSION::MINOR, VERSION::REVISION);
    else if (commandLineParser.hasFlag("-game"))
    {
        const Support::Vector<Support::String>& gameArguments = commandLineParser.getFlagArguments("-game");

        if (gameArguments.size() != 1)
        {
            CONSOLE_ERROR("No game specified.");
            commandLineParser.displayHelp(argc, argv);
            return -1;
        }

        const Support::String gameName = gameArguments[0];

        Core::SEngineInstance::MODE_NAME engineMode = Core::SEngineInstance::MODE_CLIENT;

        if (commandLineParser.hasFlag("-dedicated"))
            engineMode = Core::SEngineInstance::MODE_DEDICATED;

        // Check for the -server <ip address> flag
        Support::String targetServerIP;
        if (commandLineParser.hasFlag("-server"))
        {
            targetServerIP = commandLineParser.getFlagArguments("-server")[0];
            if (targetServerIP.empty())
            {
                CONSOLE_ERROR("No server specified.");

                commandLineParser.displayHelp(argc, argv);
                return -3;
            }

            engineMode = Core::SEngineInstance::MODE_CLIENTCONNECT;
        }

        // Create the Engine Instance
        Core::SEngineInstance* engineInstance = Core::SEngineInstance::getPointer();

        // Deal with the -mods flag
        if (commandLineParser.hasFlag("-mods"))
            engineInstance->setMods(commandLineParser.getFlagArguments("-mods"));

        engineInstance->setMode(engineMode);
        engineInstance->setTargetServer((char*)targetServerIP.c_str(), 11595);
        engineInstance->setGame(gameName);

		if (commandLineParser.hasFlag("-perfstat"))
			engineInstance->setPerfStatEnabled(true);

        engineInstance->start(argc, argv);

        // Release used memory when the start routine eventually returns
        Core::SEngineInstance::destroy();
    }
    else
        CONSOLE_INFO("Nothing to do.");

    CONSOLE_INFO("Exited successfully.");
    return 0;
}
