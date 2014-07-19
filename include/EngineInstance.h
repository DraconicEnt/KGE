/**
 *  @file Common.h
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINEINSTANCE_
#define _INCLUDE_KIARO_ENGINEINSTANCE_

#include <irrlicht/irrlicht.h>

#include <network/OutgoingClient.h>
#include <network/Server.h>

namespace Kiaro
{
    enum ENGINE_MODE
    {
        ENGINE_CLIENT = 0,
        ENGINE_CLIENTCONNECT = 1,
        ENGINE_DEDICATED = 2,
    }; // End Enum ENGINE_MODE

    class EngineInstance
    {
        public:
        static EngineInstance *Get(void);
        static void Destroy(void);

        void SetMode(const ENGINE_MODE mode);
        void SetTargetServer(Kiaro::Common::C8 *address, Kiaro::Common::U16 port);

        Kiaro::Common::U32 Run(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[]);
        void Kill(void);

        private:
        bool Running;
        ENGINE_MODE EngineMode;
        irr::IrrlichtDevice *IrrlichtDevice;

        Kiaro::Common::C8 *TargetServerAddress;
        Kiaro::Common::U16 TargetServerPort;
        Kiaro::Network::OutgoingClient *Client;
        Kiaro::Network::Server *Server;

        EngineInstance(void);
        ~EngineInstance(void);
    }; // End Class EngineInstance
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_ENGINEINSTANCE_
