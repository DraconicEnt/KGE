/**
 *  @file OutgoingClientSingleton.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_ENGINE_OUTGOINGCLIENTSINGLETON_HPP_
#define _INCLUDE_ENGINE_OUTGOINGCLIENTSINGLETON_HPP_

#include <enet/enet.h>
#include <btBulletDynamicsCommon.h>

#include <engine/EntityGroupingSingleton.hpp>
#include <network/MessageBase.hpp>

namespace irr
{
    class IrrlichtDevice;
}

namespace Kiaro
{
    namespace Engine
    {
        class BulletDebugDrawer;
    } // End NameSpace Engine

    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Network
    {
        class ServerBase;
        class PacketBase;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class OutgoingClientSingleton
        {
            // Public Methods
            public:
                static OutgoingClientSingleton *getPointer(irr::IrrlichtDevice *irrlicht = NULL);
                static void destroy(void);

                /**
                 *  @brief Empty callback function for the the OnReceivePacket event.
                 *  @param packet The received packet.
                 */
                void onReceivePacket(Kiaro::Support::BitStream &incomingStream);

                void onConnected(void);

                void onDisconnected(void);

                void onConnectFailed(void);

                bool getIsOppositeEndian(void) { return mIsOppositeEndian; }

                void disconnect(const Kiaro::Common::U32 &waitTimeMS = 3000);

                Kiaro::Common::U16 getPort(void);

                void networkUpdate(void);

                void connect(const Kiaro::Common::String &targetAddress, const Kiaro::Common::U16 &targetPort, const Kiaro::Common::U32 &wait);

                void send(Kiaro::Network::MessageBase *packet, const bool &reliable);

                const bool &getIsConnected(void);

                void dispatch(void);

            // Private Methods
            private:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                OutgoingClientSingleton(irr::IrrlichtDevice *irrlicht, ENetPeer *connecting, Kiaro::Network::ServerBase *server);

                ~OutgoingClientSingleton(void);

            // Private Members
            private:
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;

                btBroadphaseInterface *mBroadphase;
                btCollisionDispatcher *mCollisionDispatcher;
                btConstraintSolver *mConstraintSolver;
                btDefaultCollisionConfiguration *mCollisionConfiguration;
                btDiscreteDynamicsWorld *mPhysicalWorld;

                Kiaro::Engine::BulletDebugDrawer *mPhysicalDebugger;

                // Net
                bool mIsConnected;
                Kiaro::Common::U16 mPort;
                Kiaro::Common::U8 mCurrentStage;

                Kiaro::Engine::EntityGroupingSingleton *mEntityGroup;

                ENetPeer *mInternalPeer;
                ENetHost *mInternalHost;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_ENGINE_OUTGOINGCLIENTSINGLETON_HPP_
