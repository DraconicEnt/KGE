/**
 *  @file Server.h
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NETWORK_SERVER_H_
#define _INCLUDE_KIARO_NETWORK_SERVER_H_

#include "EngineCommon.h"
#include "IncomingClient.h"
#include "Packet.h"

namespace Kiaro
{
    namespace Network
    {
        //! Server class that remote hosts connect to.
        class Server
        {
            public:
            /**
             *  @brief Constructor that initializes a server instance with the given information.
             *  @param listen_address An array of c8 representing the IP address to listen on.
             *  @param listen_port A u16 representing the port number to listen on.
             *  @param clients A u32 representing the maximum number of clients to accept.
             */
            Server(const Common::C8 *listen_address, const Common::U16 listen_port, const Common::U32 clients);

            /**
			 *  @brief Standard virtual destructor.
			 *  @note The only reason this destructor is virtual is to prevent compiler warnings when deleting instances that
			 *  are children of this Server class.
			 */
            ~Server(void);

            /**
             *  @brief Signals the server to stop running.
             */
            void Stop(void);

            /**
             *  @brief Returns the current running status of the server.
             *  @return A boolean representing whether or not the server is running.
             */
            bool IsRunning(void);

            void Update(void);

            /**
             *  @brief Causes the server to handle all queued network events immediately.
             */
            void Dispatch(void);

            //! Callback function that is called upon the server's underlaying network subsystem accepting a remote host connection.
            void OnClientConnected(Kiaro::Network::IncomingClient *client);
            //! Callback function that is called upon the disconnection of a remote host.
            void OnClientDisconnected(Kiaro::Network::IncomingClient *client);

            void OnReceivePacket(Packet *packet);

			protected:
            bool is_running;

            private:
            ENetHost *Host;

            //! The Port number that we're listening on.
            Kiaro::Common::U16 Port;
            //! The Address that we're listening on.
            Kiaro::Common::U32 Address;
        };
    } // End Namespace Network
} // End Namespace Kiaro

#endif // _INCLUDE_KIARO_NETWORK_SERVER_H_
