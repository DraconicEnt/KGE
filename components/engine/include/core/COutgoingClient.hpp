/**
 *  @file COutgoingClient.hpp
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
                                
            protected:
                void processPacket(Support::CBitStream& incomingStream);
                void processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream);
                void processStageTwo(const Net::IMessage& header, Support::CBitStream& incomingStream);
                
                void onReceivePacket(Support::CBitStream& incomingStream);
        };
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_COUTGOINGCLIENT_HPP_
