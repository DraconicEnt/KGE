/**
 *  @file COutgoingClient.hpp
 *  @brief Include file declaring the COutgoingClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CORE_COUTGOINGCLIENT_HPP_
#define _INCLUDE_CORE_COUTGOINGCLIENT_HPP_

#include <net/IOutgoingClient.hpp>

namespace Kiaro
{
    namespace Core
    {
        /**
         *  @brief The COutgoingClient is an engine specialization of the Net::IOutgoingClient
         *  type. It hooks into the callback methods that Net::IOutgoingClient provides and otherwise
         *  stores any engine specific data for clients going out to a remote host.
         */
        class COutgoingClient : public Net::IOutgoingClient
        {
            public:
                /**
                 *  @brief Pure virtual callback method called by the Net::IOutgoingClient implementation
                 *  when the initial connection & authentication stage was passed.
                 */
                virtual void onConnected(void);

                /**
                 *  @brief Pure virtual callback method called by the Net::IOutgoingClient implementation
                 *  when a disconnection from the remote host has occurred.
                 */
                virtual void onDisconnected(void);

                /**
                 *  @brief Pure virtual callback method called by the Net::IOutgoingClient implementation
                 *  when a connection to a remote host has failed.
                 */
                virtual void onConnectFailed(void);

                void onAuthenticated(void);

            // Protected Methods
            protected:
                /**
                 *  @brief Used internally by the COutGoingClient implementation to process client data
                 *  in the context of stage zero.
                 *  @param header The message header to consider when processing the input.
                 *  @param incomingStream A CBitStream to unpack data from as necessary.
                 */
                void processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream);

                /**
                 *  @brief Used internally by the COutGoingClient implementation to process client data
                 *  in the context of stage two.
                 *  @param header The message header to consider when processing the input.
                 *  @param incomingStream A CBitStream to unpack data from as necessary.
                 */
                void processStageTwo(const Net::IMessage& header, Support::CBitStream& incomingStream);

                /**
                 *  @brief Called automatically by the networking subsystem when a packet has been
                 *  received by some remote machine on the same network or outside of it.
                 *  @param incomingStream A CBitStream to unpack data from as necessary.
                 */
                void onReceivePacket(Support::CBitStream& incomingStream);
        };
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_COUTGOINGCLIENT_HPP_
