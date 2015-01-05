/**
 *  @file Client.h
 *  @brief Include file defining the Kiaro::Network::OutgoingClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NETWORK_OUTGOINGCLIENTSINGLETON_H_
#define _INCLUDE_KIARO_NETWORK_OUTGOINGCLIENTSINGLETON_H_

#include <enet/enet.h>

#include <engine/Common.hpp>
#include "MessageBase.hpp"

namespace Kiaro
{
    namespace Engine
    {
        class EntityGroupingSingleton;
    }

	namespace Network
	{
        //! Class representing outgoing connections.
		class OutgoingClientBase
		{
            // Public Methods
			public:
                /**
                 *  @brief Sends arbitrary data to the remote server this Client is connected to.
                 *  @param size A u32 specifying the size of data in bytes.
                 *  @param data An arbitrary pointer to the data to be sending.
                 *  @param reliable A boolean representing whether or not this packet should be reliable. A reliable packet is slower than an unreliable packet.
                 */
                void send(Kiaro::Network::MessageBase *packet, const bool &reliable);

                /**
                 *	@brief If the client is not already connected somewhere, this tells them to connect to some server.
                 *  @param target_address A chararacter array representing the address to connect to in decimal form. This may also be a hostname. Eg: www.blah.com
                 *  @param target_port A u16 representing the UDP port to attempt to connect on.
                 *  @param wait A u32 representing the time in milliseconds to wait for. This is a blocking operation.
                 *  @note This is a blocking call, the amount of time it blocks depends on the wait variable which can be 0 for no wait.
                 */
                void connect(const std::string &targetAddress, const Kiaro::Common::U16 &targetPort, const Kiaro::Common::U32 &wait = 5000);

                /**
                 *  @brief Causes a network client to disconnect from its server.
                 *  @param wait A u32 representing the amount of time in milliseconds to wait for a registered disconnect.
                 *  @note This is a blocking call, the amount of time it blocks depends on the wait variable which can be 0 for no wait.
                 */
                void disconnect(const Kiaro::Common::U32 wait = 5000);

                /**
                 *  @brief Updates the state of this Kiaro::Network::Client.
                 */
                void update(void);

                //! Causes the client to handle all queued network events immediately.
                void dispatch(void);

                //! Callback function that is called upon the client's connection being accepted by the server's underlaying network subsystem.
                virtual void onConnected(void) = 0;
                //! Callback function that is called upon the client's disconnection from a remote host.
                virtual void onDisconnected(void) = 0;
                //! Callback function that is called upon the client's failure to connect to a remote server for any reason.
                virtual void onConnectFailed(void) = 0;
                //! Callback function that is called upon receiving a packet from the remote server.
                virtual void onReceivePacket(Kiaro::Support::BitStream &incomingStream) = 0;

                bool getIsConnected(void);

                /**
                 *  @brief Constructor accepting an incoming client connection.
                 *  @param connecting A void* pointer representing a client connection.
                 *  @note The connecting variable had to be a raw void pointer to prevent linker issues.
                 *  @note Specifying the connecting client should only be done internally by the network module.
                 */
                OutgoingClientBase(void);

                //! Standard destructor.
                ~OutgoingClientBase(void);

            // Protected Members
            protected:
                bool mIsConnected;
                Kiaro::Common::U16 mPort;
                Kiaro::Common::U8 mCurrentStage;

                Kiaro::Engine::EntityGroupingSingleton *mEntityGroup;

                ENetPeer *mInternalPeer;
                ENetHost *mInternalHost;
		};
	} // End Namespace Network
} // End Namespace Kiaro

#endif // _INCLUDE_KIARO_NETWORK_OUTGOINGCLIENTSINGLETON_H_
