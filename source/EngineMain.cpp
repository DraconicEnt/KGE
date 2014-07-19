/**
 *  @file EngineMain.cpp
 *  @brief Main source code entry point.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <iostream>

#include <physfs/physfs.h>

#include <EngineLogging.h>

#include <EngineCommon.h>
#include <EngineInstance.h>

#include <support/CommandLineParser.h>
#include <support/MapDivision.h>

#include <tests/Tests.h>

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

    // Init Logging
    Kiaro::Logging::Initialize();

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Kiaro Game Engine" << std::endl;
    //std::cout << "Kiaro Game Engine v%u.%u.%u Build %u\n", KIARO_ENGINE_VERSION_MAJOR, KIARO_ENGINE_VERSION_MINOR, KIARO_ENGINE_VERSION_REVISION, KIARO_ENGINE_BUILD_COUNT);
    std::cout << "Copyright (c) 2014 Draconic Entertainment" << std::endl;
    std::cout <<"------------------------------------------------" << std::endl;

    // NOTE (Robert MacGregor#9): Since the parser doesn't fall out of scope here, it's a pointer to dealloc manually.
    Kiaro::Support::CommandLineParser *command_parser = new Kiaro::Support::CommandLineParser(argc, argv);

    if (command_parser->HasFlag("-h"))
    {
        std::cout << "You may run " << argv[0] << " with" << std::endl;
        std::cout << "-game       <game>          - Specifies the game to run" << std::endl;
        std::cout << "-dedicated                  - Runs the engine in dedicated mode" << std::endl;

        #if ENGINE_TESTS>0
        std::cout << "-tests      <test> ...      - Runs the engine testing " << std::endl;
        #endif // ENGINE_TESTS

        #ifdef _LINUX
        printf("-Daemon     <start|stop>    - Manages the Daemon Instance\n");
        #endif
        std::cout << "-server     <ip address>    - Automatically joins a server by IP" << std::endl;

        delete command_parser;
        return 0;
    }

    #if ENGINE_TESTS>0
    if (command_parser->HasFlag("-tests"))
    {
        Kiaro::Tests::PerformTests(command_parser);
        delete command_parser;
        return 0;
    }
    #endif // ENGINE_TESTS

    // Check for the -Game <name> flag
    Kiaro::Support::String game_name = command_parser->GetFlagArgument("-game", 0);
    if (!command_parser->HasFlag("-game") || game_name.IsNull())
    {
        std::cerr << "FATAL: No game specified." << std::endl;
        std::cerr << "    Please use: -game <game>" << std::endl;
        std::cerr << "    Or for help, use: -h" << std::endl;

        delete command_parser;
        return 1;
    }

    Kiaro::ENGINE_MODE engine_mode = Kiaro::ENGINE_CLIENT;

    // Check for the -Dedicated flag
    if (command_parser->HasFlag("-dedicated"))
        engine_mode = Kiaro::ENGINE_DEDICATED;

    // Check for the -Daemon flag

    // Check for the -server <ip address> flag
    Kiaro::Support::String target_server_ip;
    if (command_parser->HasFlag("-server"))
    {
		target_server_ip = command_parser->GetFlagArgument("-server", 0);
        if (target_server_ip.IsNull())
        {
            std::cerr << "FATAL: No server specified with the server flag." << std::endl;
            std::cerr << "    Please use: -server <ip address>" << std::endl;
            std::cerr << "    Or for help, use: -h" << std::endl;

            delete command_parser;
            return 1;
        }

        engine_mode = Kiaro::ENGINE_CLIENTCONNECT;
    }

    // Create the Engine Instance
    delete command_parser;

    Kiaro::EngineInstance *engine_instance = Kiaro::EngineInstance::Get();
    engine_instance->SetMode(engine_mode);
    engine_instance->SetTargetServer(target_server_ip.C_Str(), 11595);
    engine_instance->Run(argc, argv);

    Kiaro::EngineInstance::Destroy();
    return 0;
}
