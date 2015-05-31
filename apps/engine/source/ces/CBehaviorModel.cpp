/**
 *  @file CBehaviorModel.cpp
 *  @brief Source file implementing the CES::CBehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <map>

#include <ces/CBehaviorModel.hpp>
#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace CES
    {
            CBehaviorModel::CBehaviorModel(const std::string &name)
            {

            }

            CBehaviorModel::~CBehaviorModel(void)
            {

            }

            void CBehaviorModel::setName(const std::string &name)
            {
                mName = name;
            }

            const std::string &CBehaviorModel::getName(void)
            {
                return mName;
            }

            void CBehaviorModel::packData(Support::CBitStream &out)
            {
                // TODO (Robert MacGregor#9): Transmit the associations by component identifiers
                //for (std::set<ComponentBase*>::iterator it = mComponents.begin(); it != mComponents.end(); it++)
                //    (*it).packData(out);
            }

            void CBehaviorModel::unpackData(Support::CBitStream &in)
            {

            }
    }
}
