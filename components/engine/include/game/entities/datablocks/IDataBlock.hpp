#ifndef _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_
#define _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_

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
                    class IDataBlock : public Net::INetworkPersistable
                    {
                        public:
                            template <typename childName>
                            struct SharedStatics
                            {
                                static Common::U32 sDataBlockID;
                            };

                        public:
                            /**
                             *  @brief Checks the datablock for validity.
                             *  @return A boolean representing whether or not the datablock has valid operational parameters.
                             */
                            virtual bool validate(void) = 0;
                    };

                    template <typename childName>
                    typename Common::U32 IDataBlock::SharedStatics<childName>::sDataBlockID = 0;
                } // End NameSpace DataBlocks
            }
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_IDATABLOCK_HPP_

