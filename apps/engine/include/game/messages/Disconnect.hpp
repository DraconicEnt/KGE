/**
 *  @file Disconnect.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_

#include <stdexcept>

#include <game/messages/messages.hpp>
#include <network/IMessage.hpp>

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
            class Disconnect : public Kiaro::Network::IMessage
            {
                // Public Methods
                public:
                    Disconnect(Kiaro::Support::CBitStream *in = NULL, Kiaro::Network::CClient *sender = NULL) : Network::IMessage(MESSAGE_DISCONNECT, in, sender)
                    {

                    }

                    void packData(Kiaro::Support::CBitStream &out)
                    {
                        out.writeString(mReason);

                        Kiaro::Network::IMessage::packData(out);
                    }

                    void unpackData(Kiaro::Support::CBitStream &in)
                    {
                        if (in.getSize() <= getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack Disconnect packet; too small of a payload!");

                        mReason = in.readString();
                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return sizeof(Kiaro::Common::U32);
                    }

                // Public Members
                public:
                    Kiaro::Common::String mReason;
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
