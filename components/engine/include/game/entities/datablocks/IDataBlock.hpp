/**
 *  @file IDataBlock.hpp
 *  @brief Interface class for datablocks.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entity
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_
#define _INCLUDE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_

#include <support/common.hpp>

#include <net/INetworkPersistable.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                namespace DataBlocks
                {
                    /**
                     *  @brief Interface class for all datablock types to derive from.
                     *  @details This class is used to represent all datablock types within the engine. Not only that, but it is also used
                     *  for type ID tracking within the SCoreRegistry singleton class for use in the netcode when reading datablock payloads.
                     */
                    class IDataBlock : public Net::INetworkPersistable
                    {
                        public:
                            template <typename childName>
                            struct SharedStatics
                            {
                                static Common::S32 sDataBlockID;
                            };

                            template <typename datablockName>
                            static IDataBlock* constructNetworkDataBlock(Support::CBitStream& payload)
                            {
                                IDataBlock* result = new datablockName();
                                result->unpack(payload);

                                return result;
                            }

                        public:
                            /**
                             *  @brief Checks the datablock for validity.
                             *  @return A boolean representing whether or not the datablock has valid operational parameters.
                             */
                            virtual bool validate(void) = 0;
                    };

                    template <typename childName>
                    typename Common::S32 IDataBlock::SharedStatics<childName>::sDataBlockID = -1;
                } // End NameSpace DataBlocks
            }
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_

