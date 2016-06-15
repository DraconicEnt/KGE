/**
 *  @file COutgoingClient.cpp
 */

#include <core/COutgoingClient.hpp>

#include <game/messages/messages.hpp>

#include <support/Console.hpp>

#include <net/stages.hpp>

#include <core/SEngineInstance.hpp>

namespace Kiaro
{
    namespace Core
    {
        void COutgoingClient::onConnected(void)
        {
            CONSOLE_INFO("Established connection to remote host.");

            // Dispatch our own handshake in response
            Game::Messages::HandShake handShake;
            this->send(&handShake, true);
        }

        void COutgoingClient::onDisconnected(void)
        {
        }

        void COutgoingClient::onConnectFailed(void)
        {
        }

        void COutgoingClient::handshakeHandler(Net::IIncomingClient* sender, Support::CBitStream& in)
        {
            Game::Messages::HandShake receivedHandshake;
            receivedHandshake.unpack(in);
            CONSOLE_INFOF("Server version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                          receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

            CONSOLE_INFO("Passed initial authentication.");
            mConnected = true;
            this->onAuthenticated();
            mCurrentStage = Net::STAGE_LOADING;
        }

        void COutgoingClient::scopeHandler(Net::IIncomingClient* sender, Support::CBitStream& in)
        {
            Game::Messages::Scope receivedScope;
            receivedScope.unpack(in);
        }

        void COutgoingClient::simCommitHandler(Net::IIncomingClient* sender, Support::CBitStream& in)
        {
            Game::Messages::SimCommit receivedCommit;
            receivedCommit.unpack(in);
        }

        void COutgoingClient::onReceivePacket(Support::CBitStream& incomingStream)
        {
            Core::SEngineInstance* engine = Core::SEngineInstance::getPointer();

            while (!incomingStream.isFull())
            {
                Net::IMessage basePacket;
                basePacket.unpack(incomingStream);

                Core::SEngineInstance::MessageHandlerSet::MemberDelegateFuncPtr<COutgoingClient> responder = engine->lookupClientMessageHandler(Net::STAGE_UNSTAGED, basePacket.getType());

                if (responder)
                {
                    (this->*responder)(nullptr, incomingStream);
                    continue;
                }

                // If we got to this point, look it up by the client's stage
                responder = engine->lookupClientMessageHandler(static_cast<Net::STAGE_NAME>(mCurrentStage), basePacket.getType());
                if (responder)
                {
                    (this->*responder)(nullptr, incomingStream);
                    continue;
                }

                Support::String exceptionText = "COutgoingClient: Out of stage or unknown message type encountered at stage 0 processing: ";
                exceptionText += basePacket.getType();
                throw std::out_of_range(exceptionText);
            }
        }

        void COutgoingClient::onAuthenticated(void)
        {
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
