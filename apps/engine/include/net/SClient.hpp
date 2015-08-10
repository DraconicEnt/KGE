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

#ifndef _INCLUDE_ENGINE_SCLIENT_HPP_
#define _INCLUDE_ENGINE_SCLIENT_HPP_

#include <enet/enet.h>
#include <btBulletDynamicsCommon.h>

#include <game/SGameWorld.hpp>
#include <net/IMessage.hpp>

#include <support/CBitStream.hpp>

namespace irr
{
    class IrrlichtDevice;
}

namespace Kiaro
{
    namespace Video
    {
        class CBulletDebugDrawer;
    } // End NameSpace Video

    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Net
    {
        class ServerBase;
        class PacketBase;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class SClient
        {
            // Public Methods
            public:
                static void initialize(irr::IrrlichtDevice* irrlicht);
                static SClient* getPointer(void);
                static void destroy(void);

                /**
                 *  @brief Empty callback function for the the OnReceivePacket event.
                 *  @param packet The received packet.
                 */
                void onReceivePacket(Support::CBitStream& incomingStream);

                void onConnected(void);

                void onDisconnected(void);

                void onConnectFailed(void);

                bool getIsOppositeEndian(void) { return mIsOppositeEndian; }

                void disconnect(void);

                Common::U16 getPort(void);

                void networkUpdate(void);

                void connect(const Support::String& targetAddress, const Common::U16& targetPort, const Common::U32& wait);

                void send(Net::IMessage* packet, const bool& reliable);

                const bool& getIsConnected(void);

                void dispatch(void);

            // Private Methods
            private:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                SClient(irr::IrrlichtDevice* irrlicht, ENetPeer* connecting, Net::ServerBase* server);

                ~SClient(void);

            // Private Members
            private:
                bool mIsOppositeEndian;
                ENetPeer* mInternalClient;

                btBroadphaseInterface* mBroadphase;
                btCollisionDispatcher* mCollisionDispatcher;
                btConstraintSolver* mConstraintSolver;
                btDefaultCollisionConfiguration* mCollisionConfiguration;
                btDiscreteDynamicsWorld* mPhysicalWorld;

                Video::CBulletDebugDrawer* mPhysicalDebugger;

                // Net
                bool mIsConnected;
                Common::U16 mPort;
                Common::U8 mCurrentStage;

                Game::SGameWorld* mEntityGroup;

                ENetPeer* mInternalPeer;
                ENetHost* mInternalHost;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_ENGINE_SCLIENT_HPP_
