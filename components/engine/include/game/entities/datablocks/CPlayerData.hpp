#ifndef _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_
#define _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_

#include <support/common.hpp>

#include <game/entities/datablocks/IDataBlock.hpp>

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
                    class CPlayerData : public IDataBlock
                    {
                        public:
                            Common::F32 mForwardSpeed;
                            Common::F32 mSidewaysSpeed;
                            Common::F32 mBackwardsSpeed;

                            Support::String mShapefile;
                        public:
                            CPlayerData(void);

                            bool validate(void);

                            virtual size_t getRequiredMemory(void) const;
                    };
                } // End NameSpace DataBlocks
            }
        } // End NameSpace Engine
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_
