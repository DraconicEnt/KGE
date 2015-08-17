/**
 *  @file IMessage.hpp
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NETWORK_IMESSAGE_H_
#define _INCLUDE_KIARO_NETWORK_IMESSAGE_H_

#include "support/common.hpp"

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CIncomingClient;

        namespace Messages
        {
            /**
             *  @brief Interface class used to represent messages to be sent across the network from server to client
             *  and vice versa.
             */
            class IMessage : public Support::ISerializable
            {
                // Public Members
                public:
                    static const Common::U8 sAcceptedStage = 0;

                    //! A pointer to a Client instance that sent the data. If this is NULL, then the origin was a Server.
                    CIncomingClient* mSender;

                // Private Members
                private:
                    Common::U32 mType;
                    Common::U32 mID;

                // Public Methods
                public:
                    /**
                     *  @brief Constructor that accepts a received netpacket from the underlaying networking subsystem.
                     *  @param received A packet from the internal networking subsystem to construct the class from.
                     */
                    IMessage(const Common::U32& packetType = 0, Support::CBitStream* received = NULL, CIncomingClient* sender = NULL);

                    virtual void packEverything(Support::CBitStream& out) const;

                    virtual void unpack(Support::CBitStream& in);

                    const Common::U32& getType(void) const;

                    const Common::U32& getID(void) const;

                    virtual Common::U32 getMinimumPacketPayloadLength(void);

                    size_t getRequiredMemory(void);
            };
        } // End NameSpace Messages
    } // End Namespace Net
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_NETWORK_IMESSAGE_H_