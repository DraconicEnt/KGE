/**
 *  @file COutgoingClient.cpp
 */

#include <core/COutgoingClient.hpp>

#include <game/messages/messages.hpp>

#include <support/Console.hpp>

#include <net/stages.hpp>

#include <game/messages/messages.hpp>

namespace Kiaro
{
    namespace Core
    {
        void COutgoingClient::onConnected(void)
        {
            Support::Console::write(Support::Console::MESSAGE_INFO, "COutgoingClient: Established connection to remote host.");

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
        
        void COutgoingClient::onReceivePacket(Support::CBitStream& incomingStream)
        {
            while (!incomingStream.isEmpty())
            {
                Net::IMessage basePacket;
                basePacket.unpack(incomingStream);

                switch (basePacket.getType())
                {
                    // Stageless messages
                    case Game::Messages::TYPE_DISCONNECT:
                    {
                        Game::Messages::Disconnect disconnect;
                        disconnect.unpack(incomingStream);

                        Support::Console::writef(Support::Console::MESSAGE_INFO, "IOutgoingClient: Received disconnect packet from remote host. Reason:\n%s", disconnect.mReason.data());
                        this->disconnect();

                        break;
                    }

                    // Either it's an unknown message or it's a staged message
                    default:
                    {
                        switch (mCurrentStage)
                        {
                            case Net::STAGE_AUTHENTICATION:
                            {
                                this->processStageZero(basePacket, incomingStream);
                                break;
                            }
                            
                            case Net::STAGE_LOADING:
                            {
                                this->processStageTwo(basePacket, incomingStream);
                                break;
                            }
                            
                            default:
                            {
                                Support::String exceptionText = "IOutgoingClient:  Encountered unknown stage type: ";
                                exceptionText += mCurrentStage;

                                throw std::out_of_range(exceptionText);
                                break;
                            }
                        }
                    }
                }
            }
        }

        void COutgoingClient::processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream)
        {
            switch (header.getType())
            {
                case Game::Messages::TYPE_HANDSHAKE:
                {
                    Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.unpack(incomingStream);

                    Support::Console::writef(Support::Console::MESSAGE_INFO, "IOutgoingClient: Server version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                    receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

                    Support::Console::write(Support::Console::MESSAGE_INFO, "IOutgoingClient: Passed initial authentication.");

                    mIsConnected = true;
                    this->onConnected();

                    mCurrentStage = Net::STAGE_LOADING;

                    break;
                }

                // Out of stage packet or unknown type
                default:
                {
                    Support::String exceptionText = "IOutgoingClient: Out of stage or unknown message type encountered at stage 0 processing: ";
                    exceptionText += header.getType();

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }

        void COutgoingClient::processStageTwo(const Net::IMessage& header, Support::CBitStream& incomingStream)
        {
            switch (header.getType())
            {
                case Game::Messages::TYPE_SCOPE:
                {
                    Game::Messages::Scope receivedScope;
                    receivedScope.unpack(incomingStream);

                    break;
                }

                case Game::Messages::TYPE_SIMCOMMIT:
                {
                    Game::Messages::SimCommit receivedCommit;
                    receivedCommit.unpack(incomingStream);

                    break;
                }

                // Out of stage packet or unknown type
                default:
                {
                    Support::String exceptionText = "IOutgoingClient: Out of stage or unknown message type encountered at stage 2 processing: ";
                    exceptionText += header.getType();

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
