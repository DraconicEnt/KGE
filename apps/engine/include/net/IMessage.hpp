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

#include "core/common.hpp"

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CClient;

        //! Class representing a packet that has or is ready to traverse across the network to a remote host.
        class IMessage : public Kiaro::Support::ISerializable
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor that accepts a received netpacket from the underlaying networking subsystem.
                 *  @param received A packet from the internal networking subsystem to construct the class from.
                 */
                IMessage(const Kiaro::Common::U32 &packetType = 0, Kiaro::Support::CBitStream *received = NULL, CClient *sender = NULL) : mID(0),
                mType(packetType), mSender(sender)
                {
                    //if (received)
                    //    UnpackData(*received);
                }

                virtual void writeTo(Kiaro::Support::CBitStream &out) const
                {
                    static Kiaro::Common::U32 sLastPacketID = 0;

                    out << mType << sLastPacketID;
                    // TODO (Robert MacGregor#9): Sequencing?
                   // mID = sLastPacketID;

                    sLastPacketID++;
                }

                virtual void extractFrom(Support::CBitStream &in)
                {
                    in >> mID;
                    in >> mType;
                }

                Common::U32 getType(void) { return mType; }

                Common::U32 getID(void) { return mID; }

                virtual Common::U32 getMinimumPacketPayloadLength(void)
                {
                    return sizeof(Common::U32) * 2;
                }

                size_t getRequiredMemory(void)
                {
                    return sizeof(Common::U32) * 2;
                }

            // Public Members
            public:
                static const Kiaro::Common::U8 sAcceptedStage = 0;

                //! A pointer to a Client instance that sent the data. If this is NULL, then the origin was a Server.
                Kiaro::Net::CClient *mSender;

            // Private Members
            private:
                Kiaro::Common::U32 mType;
                Kiaro::Common::U32 mID;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_NETWORK_IMESSAGE_H_