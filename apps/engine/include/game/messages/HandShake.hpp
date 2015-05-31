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

#include <game/messages/messages.hpp>
#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Network
    {
        class IncomingClient;
        class NetworkedEntity;
    } // End NameSpace Network

    namespace Game
    {
        namespace Messages
        {
            class HandShake : public Kiaro::Net::IMessage
            {
                // Public Methods
                public:
                    HandShake(Kiaro::Support::CBitStream *in = NULL, Kiaro::Net::CClient *sender = NULL) : Net::IMessage(MESSAGE_HANDSHAKE, in, sender)
                    {

                    }

                    void writeTo(Kiaro::Support::CBitStream &out) const
                    {
                        out << mVersionMajor << mVersionMinor << mVersionRevision << mVersionBuild;

                        Kiaro::Net::IMessage::writeTo(out);
                    }

                    void extractFrom(Kiaro::Support::CBitStream &in)
                    {
                        if (in.getWrittenLength() < getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack HandShake packet; too small of a payload!");

                        in >> mVersionBuild >> mVersionRevision >> mVersionMinor >> mVersionMajor;
                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return sizeof(Kiaro::Common::U32) + (sizeof(Kiaro::Common::U8) * 3);
                    }

                    size_t getRequiredMemory(void)
                    {
                        return (sizeof(Common::U8) * 3) + sizeof(Common::U32) + Net::IMessage::getRequiredMemory();
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
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_HANDSHAKE_HPP_
