/**
 *  @file SSettingsRegistry.cpp
 */

#include <allegro5/allegro.h>

#include <support/Console.hpp>
#include <support/common.hpp>
#include <support/String.hpp>
#include <support/support.hpp>

#include <support/SSettingsRegistry.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SSettingsRegistry* sInstance = NULL;

        SSettingsRegistry* SSettingsRegistry::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SSettingsRegistry();

            return sInstance;
        }

        void SSettingsRegistry::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = NULL;
            }
        }

        void SSettingsRegistry::setDefaultValues(void)
        {
            this->setValue("Server::ListenAddress", Support::String("0.0.0.0"));
            this->setValue("Server::ListenPort", Common::U16(11595));
            this->setValue("Server::MaximumClientCount", Common::U32(32));

            this->setValue("Video::Fullscreen", true);
            this->setValue("Video::Resolution", irr::core::dimension2d<Common::U32>(640, 480));
            this->setValue("Video::ActiveFPS", Common::U16(60));
            this->setValue("Video::InactiveFPS", Common::U16(15));

            this->setValue("System::WorkerThreadCount", Common::U8(6));
        }

        SSettingsRegistry::SSettingsRegistry(void)
        {
            this->setDefaultValues();

            ALLEGRO_CONFIG* config = al_load_config_file("config.cfg");

            if (!config)
                Console::write(Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to load config.cfg, using default values.");
            else
            {
                const Support::Regex numberRegex("[0-9]+", Support::RegexConstants::Extended);
                const Support::Regex resolutionRegex("[0-9]+x[0-9]+", Support::RegexConstants::Extended);
                const Support::Regex addressRegex("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}", Support::RegexConstants::Extended);

                // Listen port?
                Common::U16 listenPort = 11595;
                const Common::C8* configValue = al_get_config_value(config, "Server", "ListenPort");
                if (configValue && Support::RegexMatch(configValue, numberRegex))
                    listenPort = atoi(configValue);
                else
                    Support::Console::writef(Support::Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to parse Server::ListenPort config ('%s')! Using default value.", configValue);

                // Maximum client count?
                Common::U32 maximumClientCount = 32;
                // FIXME (Robert MacGregor#9): Force GCC 4.9 when compiling this?
                configValue = al_get_config_value(config, "Server", "MaximumClientCount");
                if (configValue && Support::RegexMatch(configValue, numberRegex))
                    maximumClientCount = atoi(configValue);
                else
                    Support::Console::writef(Support::Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to parse Server::MaximumClientCount config ('%s')! Using default value.", configValue);

                // Listen Address?
                configValue = al_get_config_value(config, "Server", "ListenAddress");
                Support::String listenAddress = "0.0.0.0";
                if (configValue && Support::RegexMatch(configValue, addressRegex))
                    listenAddress = configValue;
                else
                    Support::Console::writef(Support::Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to parse Server::ListenAddress config ('%s')! Using default value.", configValue);

                // Full screen?
                bool fullScreen = true;
                configValue = al_get_config_value(config, "Video", "Fullscreen");
                if (configValue)
                    fullScreen = atoi(configValue);

                // Active & inactive FPS?
                Common::U16 activeFPS = 60;
                configValue = al_get_config_value(config, "Video", "ActiveFPS");
                if (configValue)
                    activeFPS = atoi(configValue);

                Common::U16 inactiveFPS = 15;
                configValue = al_get_config_value(config, "Video", "InactiveFPS");
                if (configValue)
                    inactiveFPS = atoi(configValue);

                // Resolution?
                Support::Dimension2DU resolution(640, 480);
                configValue = al_get_config_value(config, "Video", "Resolution");
                if (configValue && Support::RegexMatch(configValue, resolutionRegex))
                {
                    // Make sure the resolution follows the pattern we want and extract the width & height if so.
                    const Support::String resolutionString = configValue;
                    const size_t splitLocation = resolutionString.find("x");

                    const Support::String widthString = resolutionString.substr(0, splitLocation);
                    const Support::String heightString = resolutionString.substr(splitLocation + 1, resolutionString.length());

                    resolution = Support::Dimension2DU(atoi(widthString.data()), atoi(heightString.data()));
                }
                else
                    Support::Console::writef(Support::Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to parse Video::Resolution config ('%s')! Using default value.", configValue);

                // Worker thread count?
                Common::U8 workerThreadCount = 6;
                configValue = al_get_config_value(config, "System", "WorkerThreadCount");
                if (configValue)
                    workerThreadCount = atoi(configValue);

                this->setValue("Server::ListenAddress", listenAddress);
                this->setValue("Server::ListenPort", listenPort);
                this->setValue("Server::MaximumClientCount", maximumClientCount);

                this->setValue("Video::Fullscreen", fullScreen);
                this->setValue("Video::Resolution", resolution);
                this->setValue("Video::ActiveFPS", activeFPS);
                this->setValue("Video::InactiveFPS", inactiveFPS);

                this->setValue("System::WorkerThreadCount", workerThreadCount);

                al_destroy_config(config);
                Console::write(Console::MESSAGE_INFO, "SSettingsRegistry: Loaded config.cfg.");
            }
        }

        SSettingsRegistry::~SSettingsRegistry(void)
        {
            // Dump our current config to the file before destroying
            ALLEGRO_CONFIG* config = al_create_config();

            if (config)
            {
                Common::C8 tempBuffer[256];
                sprintf(tempBuffer, "Automatically generated by KGE %u.%u.%u", VERSION::MAJOR, VERSION::MINOR, VERSION::REVISION);

                // Header comments---------------------
                al_add_config_comment(config, NULL, "Configuration values for the operation of the KGE");
                al_add_config_comment(config, NULL, tempBuffer);

                // Write server section----------------
                al_add_config_section(config, "Server");
                al_add_config_comment(config, "Server", "Configuration values for the server end");
                al_add_config_comment(config, "Server", "ListenAddress specifies what IP address the server will bind to");
                al_set_config_value(config, "Server", "ListenAddress", this->getValue<Support::String>("Server::ListenAddress").data());

                // Listen Port
                sprintf(tempBuffer, "%u", this->getValue<Common::U16>("Server::ListenPort"));
                al_add_config_comment(config, "Server", "ListenPort specifies what port number the server will listen on");
                al_set_config_value(config, "Server", "ListenPort", tempBuffer);

                // Maximum Client Count
                sprintf(tempBuffer, "%u", this->getValue<Common::U32>("Server::MaximumClientCount"));
                al_add_config_comment(config, "Server", "MaximumClientCount specifies the maximum number of remote clients allowed");
                al_set_config_value(config, "Server", "MaximumClientCount", tempBuffer);

                // Write video section-----------------------
                al_add_config_section(config, "Video");
                al_add_config_comment(config, "Video", "Video output configuration");
                al_add_config_comment(config, "Video", "Resolution controls the window resolution of the engine");

                Support::Dimension2DU resolution = this->getValue<Support::Dimension2DU>("Video::Resolution");
                sprintf(tempBuffer, "%ux%u", resolution.Width, resolution.Height);
                al_set_config_value(config, "Video", "Resolution", tempBuffer);
                al_add_config_comment(config, "Video", "Fullscreen controls whether or not the engine will run full screen");
                al_set_config_value(config, "Video", "Fullscreen", "1");

                sprintf(tempBuffer, "%u", this->getValue<Common::U16>("Video::InactiveFPS"));
                al_add_config_comment(config, "Video", "Inactive FPS determines what framerate the engine will render at when the window is inactive.");
                al_add_config_comment(config, "Video", "Setting this to a high value does not guarantee that FPS to actually be achieved.");
                al_add_config_comment(config, "Video", "This should generally be a low value to help system performance when the game isn't focused.");

                al_set_config_value(config, "Video", "InactiveFPS", tempBuffer);

                sprintf(tempBuffer, "%u", this->getValue<Common::U16>("Video::ActiveFPS"));
                al_add_config_comment(config, "Video", "Active FPS determines what framerate the engine will render at when the window is active.");
                al_add_config_comment(config, "Video", "Setting this to a high value does not guarantee that FPS to actually be achieved.");
                al_set_config_value(config, "Video", "ActiveFPS", tempBuffer);

                // Write system section----------------------
                al_add_config_section(config, "System");
                al_add_config_comment(config, "System", "Core system configuration.");

                // Worder Thread Count
                sprintf(tempBuffer, "%u", this->getValue<Common::U8>("System::WorkerThreadCount"));
                al_add_config_comment(config, "System", "WorkerThreadCount dictates how many worker threads will be created for the asynchronous task manager");
                al_add_config_comment(config, "System", "If WorkerThreadCount=0 then the async tasker is disabled and such tasks will be handled synchronously");
                al_set_config_value(config, "System", "WorkerThreadCount", tempBuffer);

                // We're done
                al_save_config_file("config.cfg", config);
                al_destroy_config(config);

                Console::write(Console::MESSAGE_INFO, "SSettingsRegistry: Wrote new configuration file.");
            }
            else
                Console::write(Console::MESSAGE_ERROR, "SSettingsRegistry: Failed to acquire Allegro config handle!");

            // Make sure we clear the heap elements
            for (auto it = mStoredProperties.begin(); it != mStoredProperties.end(); it++)
                free(it->second.first);
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
