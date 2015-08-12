/**
 *  @file SClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_ENGINE_COUTGOINGCLIENT_HPP_
#define _INCLUDE_ENGINE_COUTGOINGCLIENT_HPP_

#include <enet/enet.h>
//#include <btBulletDynamicsCommon.h>

//#include <game/SGameWorld.hpp>

#include <net/IMessage.hpp>
#include <net/IServer.hpp>

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Net
    {
        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class IOutgoingClient
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                IOutgoingClient(void);

                //! Standard destructor.
                ~IOutgoingClient(void);

                /**
                 *  @brief Pure virtual callback method for when the IOutgoingClient has made a successful
                 *  connection to a remote host.
                 */
                virtual void onConnected(void) = 0;

                /**
                 *  @brief Pure virtual callback method for when the IOutgoingClient has disconnected from
                 *  the remote host.
                 */
                virtual void onDisconnected(void) = 0;

                /**
                 *  @brief Pure virtual callback method for when the IOutgoingClient attempted to connect to
                 *  a remote host but has failed the connection process.
                 */
                virtual void onConnectFailed(void) = 0;

                /**
                 *  @brief Pure virtual callback method for when a packet has been received by the internal
                 *  IOutgoingClient implementation.
                 *  @param incomingStream A CBitStream capable of indexing the raw memory block that was part
                 *  of the incoming packet's payload.
                 */
                virtual void onReceivePacket(Support::CBitStream& incomingStream) = 0;

                bool getIsOppositeEndian(void) { return mIsOppositeEndian; }

                /**
                 *  @brief Signals to the IOutgoingClient from the remote host the next chance it gets.
                 */
                void disconnect(void);

                /**
                 *  @brief Signals to the IOutgoingClient to connect to a remote host with the given information.
                 *  @param hostName The hostname of the target server to connect to. This may either be a DNS name or
                 *  an IP address in the form x.x.x.x without the port number attached.
                 *  @param targetPort The port number to attempt the connection on.
                 *  @param wait The wait time on milliseconds to wait.
                 */
                void connect(const Support::String& hostName, const Common::U16& targetPort, const Common::U32& wait);

                /**
                 *  @brief Returns the port number that the IOutgoingClient is connected to on the remote host.
                 *  @return A Common::U16 representing the port number that the IOutgoingClient is connected to
                 *  on the remote host.
                 */
                Common::U16 getPort(void);

                void update(void);

                void send(IMessage* packet, const bool& reliable);

                const bool& isConnected(void);

                void dispatch(void);

            // Private Members
            private:
                bool mIsOppositeEndian;
                ENetPeer* mInternalClient;

               // btBroadphaseInterface* mBroadphase;
               // btCollisionDispatcher* mCollisionDispatcher;
               // btConstraintSolver* mConstraintSolver;
               // btDefaultCollisionConfiguration* mCollisionConfiguration;
               // btDiscreteDynamicsWorld* mPhysicalWorld;

               // Video::CBulletDebugDrawer* mPhysicalDebugger;

                // Net
                bool mIsConnected;
                Common::U16 mPort;
                Common::U8 mCurrentStage;

                //Game::SGameWorld* mEntityGroup;

                ENetPeer* mInternalPeer;
                ENetHost* mInternalHost;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_ENGINE_COUTGOINGCLIENT_HPP_
