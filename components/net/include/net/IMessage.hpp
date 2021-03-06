/**
 *  @file IMessage.hpp
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_NETWORK_IMESSAGE_H_
#define _INCLUDE_KIARO_NETWORK_IMESSAGE_H_

#include "support/common.hpp"

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;

        /**
         *  @brief Interface class used to represent messages to be sent across the network from server to client
         *  and vice versa.
         */
        class IMessage : public Support::ISerializable
        {
            // Public Members
            public:
                //! A pointer to a Client instance that sent the data. If this is NULL, then the origin was a Server.
                IIncomingClient* mSender;

                template <typename className>
                static IMessage* constructMessage(Support::CBitStream& in)
                {
                    className* result = new className();
                    result->unpack(in);

                    return result;
                }

                template <typename childName>
                struct SharedStatics
                {
                    static Common::S32 sMessageID;
                };

            // Private Members
            private:
                //! The type identifier of this message.
                Common::U32 mType;
                //! The sequencing ID of this message.
                Common::U32 mID;

                // Public Methods
            public:
                /**
                 *  @brief Constructor that accepts a received netpacket from the underlaying networking subsystem.
                 *  @param received A packet from the internal networking subsystem to construct the class from.
                 */
                IMessage(Support::CBitStream* received = nullptr, IIncomingClient* sender = nullptr);

                virtual void packEverything(Support::CBitStream& out) const;

                virtual void unpack(Support::CBitStream& in);

                template <typename messageType>
                void packBaseData(Support::CBitStream& out) const
                {
                    static Common::U32 sLastPacketID = 0;
                    out << IMessage::SharedStatics<messageType>::sMessageID << (sLastPacketID++);
                    // TODO (Robert MacGregor#9): Sequencing?
                    // mID = sLastPacketID;
                }

                Common::U32 getType(void) const;

                Common::U32 getID(void) const;

                virtual size_t getMinimumPacketPayloadLength(void) const;

                virtual size_t getRequiredMemory(void) const;
        };

        template <typename childName>
        typename Common::S32 IMessage::SharedStatics<childName>::sMessageID = -1;
    } // End Namespace Net
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_NETWORK_IMESSAGE_H_
