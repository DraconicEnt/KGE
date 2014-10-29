/**
 *  @file HandShake.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_PACKETS_HANDSHAKE_HPP_
#define _INCLUDE_KIARO_GAME_PACKETS_HANDSHAKE_HPP_

#include <stdexcept>

#include <network/PacketBase.hpp>

namespace Kiaro
{
    namespace Network
    {
        class IncomingClient;
        class NetworkedEntity;
    } // End NameSpace Network

    namespace Game
    {
        namespace Packets
        {
            class HandShake : public Kiaro::Network::PacketBase
            {
                // Public Methods
                public:
                    HandShake(Kiaro::Support::BitStream *in = NULL, Kiaro::Network::IncomingClientBase *sender = NULL) : Network::PacketBase(PACKET_HANDSHAKE, in, sender)
                    {

                    }

                    void packData(Kiaro::Support::BitStream &out)
                    {
                        out.writeU8(mVersionMajor);
                        out.writeU8(mVersionMinor);
                        out.writeU8(mVersionRevision);
                        out.writeU32(mVersionBuild);

                        Kiaro::Network::PacketBase::packData(out);
                    }

                    void unpackData(Kiaro::Support::BitStream &in)
                    {
                        if (in.length() <= getMinimumPacketPayloadLength())
                            throw std::runtime_error("Unable to unpack HandShake packet; too small of a payload!");

                        mVersionBuild = in.readU32();
                        mVersionRevision = in.readU8();
                        mVersionMinor = in.readU8();
                        mVersionMajor = in.readU8();
                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return sizeof(Kiaro::Common::U32) + (sizeof(Kiaro::Common::U8) * 3);
                    }

                // Public Members
                public:
                    //! The major version of the engine.
                    Kiaro::Common::U8 mVersionMajor;
                    //! The minor version of the engine.
                    Kiaro::Common::U8 mVersionMinor;
                    //! The revision of the engine.
                    Kiaro::Common::U8 mVersionRevision;
                    //! The build # of the engine.
                    Kiaro::Common::U32 mVersionBuild;

                    static const Kiaro::Common::U8 sAcceptedStage = 1;
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_HANDSHAKE_HPP_
