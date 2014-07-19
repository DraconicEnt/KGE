/**
 *  @file EngineInstance.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <EngineConfig.h>
#include <EngineInstance.h>

#include <EngineLogging.h>

namespace Kiaro
{
    Kiaro::EngineInstance *EngineInstance_Singleton = 0x00;

    EngineInstance *EngineInstance::Get(void)
    {
        if (!EngineInstance_Singleton)
            EngineInstance_Singleton = new EngineInstance;

        return EngineInstance_Singleton;
    }

    void EngineInstance::Destroy(void)
    {
        if (EngineInstance_Singleton)
        {
            EngineInstance_Singleton->Kill();
            delete EngineInstance_Singleton;
        }

        EngineInstance_Singleton = 0x00;
    }

    void EngineInstance::SetMode(const ENGINE_MODE mode)
    {
        this->EngineMode = mode;
    }

    void EngineInstance::SetTargetServer(Kiaro::Common::C8 *address, Kiaro::Common::U16 port)
    {
        this->TargetServerAddress = address;
        this->TargetServerPort = port;
    }

    Kiaro::Common::U32 EngineInstance::Run(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[])
    {
        // Handle Execution Flag
        irr::video::E_DRIVER_TYPE video_driver = irr::video::EDT_OPENGL;

        // Init ENet
        enet_initialize();

        switch (this->EngineMode)
        {
            case Kiaro::ENGINE_CLIENT:
                this->Client = Kiaro::Network::OutgoingClient::Get();
                break;
            case Kiaro::ENGINE_CLIENTCONNECT:
                this->Client = Kiaro::Network::OutgoingClient::Get();
                this->Client->Connect(this->TargetServerAddress, this->TargetServerPort, 5000);

                if (!this->Client->IsConnected())
                {
                    Kiaro::Logging::Write("EngineInstance: Failed to connect to remote host with server flag", Kiaro::Logging::LEVEL_ERROR);

                    // NOTE (Robert MacGregor#1): Returning here will destroy the allocated client at engine deinit
                    return 1;
                }
                break;
            case Kiaro::ENGINE_DEDICATED:
                video_driver = irr::video::EDT_NULL;

                this->Server = new Kiaro::Network::Server("0.0.0.0", 11595, 32);
                break;
        }

        // Start up Irrlicht
        this->IrrlichtDevice = irr::createDevice(video_driver);

        // Start the Loop
        time_t last_time = clock();
        time_t delta_time = 0;

        while (this->Running)
        {
            time_t current_time = clock();
            delta_time = current_time - delta_time;
            last_time = current_time;

            if (this->Client)
                this->Client->Update();

            if (this->Server)
                this->Server->Update();

            // NOTE (Robert MacGregor#1): TickRate
            boost::this_thread::sleep(boost::posix_time::milliseconds(ENGINE_TICKRATE));
        }
    }

    void EngineInstance::Kill(void)
    {
        if (!this->Running)
            return;

        Kiaro::Logging::Write("EngineInstance: Engine killed via Kill()");

        this->Running = false;
    }

    EngineInstance::EngineInstance(void) : EngineMode(Kiaro::ENGINE_CLIENT), IrrlichtDevice(0x00), TargetServerAddress("127.0.0.1"), TargetServerPort(11595), Client(0x00), Server(0x00)
    {

    }

    EngineInstance::~EngineInstance(void)
    {
        Kiaro::Logging::Write("EngineInstance: Deinitializing ...");

        if (this->Client)
        {
            Kiaro::Network::OutgoingClient::Destroy();
            this->Client = 0x00;
        }

        if (this->Server)
        {
            delete this->Server;
            this->Server = 0x00;
        }

        PHYSFS_deinit();
        enet_deinitialize();
    }
}
