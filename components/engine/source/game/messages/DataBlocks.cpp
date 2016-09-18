 /**
 *  @file DataBlocks.cpp
 *  @brief Source file implementing the DataBlocks message class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdexcept>

#include <game/messages/DataBlocks.hpp>
#include <net/IIncomingClient.hpp>
#include <support/Console.hpp>

#include <support/support.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Messages
            {
                DataBlocks::DataBlocks(Support::CBitStream* in, Net::IIncomingClient* sender) : IMessage(in, sender), mDataBlockCount(0)
                {
                }

                void DataBlocks::packEverything(Support::CBitStream& out) const
                {
                    IMessage::packBaseData<DataBlocks>(out);

                    out << mDataBlockCount;
                }

                void DataBlocks::unpack(Support::CBitStream& in)
                {
                    if (in.getSize() - in.getPointer() < this->getMinimumPacketPayloadLength())
                        Support::throwException<std::underflow_error>("Unable to unpack HandShake packet; too small of a payload!");

                    in >> mDataBlockCount;
                    CONSOLE_DEBUGF("Received %u datablocks.", mDataBlockCount);

                    // Now we unpack the datablocks
                    for (Common::U32 iteration = 0; iteration < mDataBlockCount; ++iteration)
                    {

                    }
                }

                size_t DataBlocks::getMinimumPacketPayloadLength(void) const
                {
                    return sizeof(Common::U32);
                }

                size_t DataBlocks::getRequiredMemory(void) const
                {
                    return sizeof(Common::U32) + IMessage::getRequiredMemory();
                }
            } // End NameSpace Messages
        }
    } // End NameSpace Game
} // End NameSpace Kiaro


