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

#include <net/messages/IMessage.hpp>
#include <net/IServer.hpp>

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Messages
    {
        class IMessage;
    } // End NameSpace Messages

    namespace Net
    {
        /**
         *  @brief The IOutgoingClient type is an interface type used for representing outgoing clients
         *  connecting to a remote host somewhere.
         */
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
                //virtual void onReceivePacket(Support::CBitStream& incomingStream) = 0;

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

                void send(Messages::IMessage* packet, const bool& reliable);

                const bool& isConnected(void);

                void dispatch(void);

            // Private Methods
            private:
                /**
                 *  @brief An internally called method used to process packet payloads specified in a CBitStream.
                 *  @param incomingStream A CBitStream indexing the packet payload to use.
                 */
                void processPacket(Support::CBitStream& incomingStream);

                /**
                 *  @brief An internally called method used to process packet payloads specified in a CBitStream in
                 *  respect to stage zero.
                 *  @param incomingStream A CBitStream indexing the packet payload to use.
                 */
                void processStageZero(const Messages::IMessage& header, Support::CBitStream& incomingStream);

                /**
                 *  @brief An internally called method used to process packet payloads specified in a CBitStream in
                 *  respect to stage two.
                 *  @param incomingStream A CBitStream indexing the packet payload to use.
                 */
                void processStageTwo(const Messages::IMessage& header, Support::CBitStream& incomingStream);

                //! Internally called method when the IOutgoingClient connected to a remote host.
                void internalOnConnected(void);

            // Private Members
            private:
                bool mIsOppositeEndian;

               // btBroadphaseInterface* mBroadphase;
               // btCollisionDispatcher* mCollisionDispatcher;
               // btConstraintSolver* mConstraintSolver;
               // btDefaultCollisionConfiguration* mCollisionConfiguration;
               // btDiscreteDynamicsWorld* mPhysicalWorld;

               // Video::CBulletDebugDrawer* mPhysicalDebugger;

                /**
                 *  @brief What port is the IOutgoingClient currently connected on?
                 *  @note This value is irrelevant if !mInternalPeer.
                 */
                Common::U16 mPort;

                /**
                 *  @brief What is the current connection stage that the IOutgoingClient is in?
                 *  @note This value is irrelvant if !mInternalPeer.
                 */
                Common::U8 mCurrentStage;

                bool mIsConnected;

                //Game::SGameWorld* mEntityGroup;

                //! A pointer to the internally utilized ENetPeer.
                ENetPeer* mInternalPeer;

                //! A pointer to the internally utilized ENetHost.
                ENetHost* mInternalHost;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_ENGINE_COUTGOINGCLIENT_HPP_