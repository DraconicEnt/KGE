/**
 *  @file CPlayerData.hpp
 *  @brief Include file declaring the CPlayerData class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entity
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_
#define _INCLUDE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_

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
                    //! A datablock for use with the CPlayer entity type.
                    class CPlayerData : public IDataBlock
                    {
                        public:
                            //! How fast the player object will run forward.
                            Common::F32 mForwardSpeed;
                            //! How fast the player object will sidestep.
                            Common::F32 mSidewaysSpeed;
                            //! How fast the player object will step backwards.
                            Common::F32 mBackwardsSpeed;
                            //! The path to the shape data to use.
                            Support::String mShapefile;
                        public:
                            //! Parameter-less constructor.
                            CPlayerData(void);

                            /**
                             *  @brief Validates whether or not the currently stored values on this datablock
                             *  are valid.
                             *  @return True for valid, false otherwise.
                             */
                            bool validate(void);

                            /**
                             *  @brief Returns the number of bytes required to serialize this CPlayerData.
                             *  @return The number of bytes required to serialize.
                             */
                            virtual size_t getRequiredMemory(void) const;

                            /**
                             *  @brief Called to register networked properties for this datablock type.
                             */
                            virtual void registerNetworkedProperties(void);
                    };
                } // End NameSpace DataBlocks
            }
        } // End NameSpace Engine
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_ENTITIES_DATABLOCKS_CPLAYERDATA_HPP_
