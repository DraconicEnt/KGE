/**
 *  @file CPlayerData.cpp
 *  @brief Source file implementing the CPlayerData methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entity
 *  @copyright (c) 2016 Draconic Entity
 */
#include <support/Console.hpp>

#include <game/entities/datablocks/CPlayerData.hpp>

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
                    CPlayerData::CPlayerData(void)
                    {
                        this->registerNetworkedProperties();
                    }

                    bool CPlayerData::validate(void)
                    {
                        if (mForwardSpeed <= 0.0f)
                        {
                            CONSOLE_ERRORF("Invalid forwardSpeed! %f <= 0.0f", mForwardSpeed);
                            return false;
                        }

                        if (mSidewaysSpeed <= 0.0f)
                        {
                            CONSOLE_ERRORF("Invalid sidewaysSpeed! %f <= 0.0f", mSidewaysSpeed);
                            return false;
                        }

                        if (mBackwardsSpeed <= 0.0f)
                        {
                            CONSOLE_ERRORF("Invalid backwardSpeed! %f <= 0.0f", mBackwardsSpeed);
                            return false;
                        }

                        return true;
                    }

                    size_t CPlayerData::getRequiredMemory(void) const
                    {
                        return (sizeof(Common::F32) * 3) + mShapefile.size() + 1;
                    }

                    void CPlayerData::registerNetworkedProperties(void)
                    {
                        this->addNetworkedProperty("forwardSpeed", mForwardSpeed);
                        this->addNetworkedProperty("sidewaysSpeed", mSidewaysSpeed);
                        this->addNetworkedProperty("backwardSpeed", mBackwardsSpeed);
                        this->addNetworkedProperty("shapefile", mShapefile);
                    }
                }
            } // End NameSpace DataBlocks
        }
    } // End NameSpace Engine
} // End NameSpace Kiaro
