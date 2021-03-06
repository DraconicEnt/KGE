/**
 *  @file HandShake.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_
#define _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_

#include <stdexcept>

#include <support/common.hpp>
#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;
    }

    namespace Engine
    {
        namespace Game
        {
            namespace Messages
            {
                /**
                 *  @brief The handshake class represents your basic authentication message with the engine. Whatever information is required
                 *  for a client to be allowed into the game session should be passed in on this message.
                 */
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
                        //! The number of datablocks waiting to be received.
                        Common::U32 mDataBlockCount;

                    // Public Methods
                    public:
                        HandShake(Support::CBitStream* in = nullptr, Net::IIncomingClient* sender = nullptr);
                        virtual void packEverything(Support::CBitStream& out) const;

                        void unpack(Support::CBitStream& in);

                        virtual size_t getMinimumPacketPayloadLength(void) const;

                        virtual size_t getRequiredMemory(void) const;
                };
            } // End NameSpace Messages
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_MESSAGES_HANDSHAKE_HPP_
