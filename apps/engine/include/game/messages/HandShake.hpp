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

#ifndef _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_
#define _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_

#include <stdexcept>

#include <core/common.hpp>
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
            class HandShake : public Net::IMessage
            {
                // Private Members
                public:
                    //! The major version of the engine.
                    Common::U8 mVersionMajor;
                    //! The minor version of the engine.
                    Common::U8 mVersionMinor;
                    //! The revision of the engine.
                    Common::U8 mVersionRevision;
                    //! The build # of the engine.
                    Common::U32 mVersionBuild;
                    //! The network protocol version.
                    Common::U32 mProtocolVersion;

                // Public Methods
                public:
                    HandShake(Support::CBitStream* in = NULL, Net::CClient* sender = NULL) : Net::IMessage(MESSAGE_HANDSHAKE, in, sender),
                    mVersionMajor(VERSION::MAJOR), mVersionMinor(VERSION::MINOR), mVersionRevision(VERSION::REVISION),
                    mVersionBuild(VERSION::BUILD), mProtocolVersion(VERSION::PROTOCOL)
                    {

                    }

                    void writeTo(Support::CBitStream& out) const
                    {
                        out << mVersionMajor << mVersionMinor << mVersionRevision << mVersionBuild << mProtocolVersion;

                        Net::IMessage::writeTo(out);
                    }

                    void extractFrom(Support::CBitStream& in)
                    {
                        if (in.getWrittenLength() < this->getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack HandShake packet; too small of a payload!");

                        in >> mProtocolVersion >> mVersionBuild >> mVersionRevision >> mVersionMinor >> mVersionMajor;
                    }

                    Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return (sizeof(Common::U32) * 2) + (sizeof(Common::U8) * 3);
                    }

                    size_t getRequiredMemory(void)
                    {
                        return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + Net::IMessage::getRequiredMemory();
                    }
            };
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_
