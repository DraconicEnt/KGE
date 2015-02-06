/**
 *  @file BehaviorModel.cpp
 *  @brief Source file implementating the Kiaro::CES::BehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <map>

#include <ces/BehaviorModel.hpp>

namespace Kiaro
{
    namespace CES
    {
            BehaviorModel::BehaviorModel(const std::string &name)
            {

            }

            BehaviorModel::~BehaviorModel(void)
            {

            }

            void BehaviorModel::setName(const std::string &name)
            {
                mName = name;
            }

            const std::string &BehaviorModel::getName(void)
            {
                return mName;
            }

            void BehaviorModel::packData(Kiaro::Support::BitStream &out)
            {

            }

            void BehaviorModel::unpackData(Kiaro::Support::BitStream &in)
            {

            }
    }
}
