/**
 *  @file CBehaviorModel.hpp
 *  @brief Include file defining the Kiaro::CES::CBehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_
#define _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_

#include <set>
#include <string>

#include <core/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace CES
    {
        class IComponent;

        class CBehaviorModel : public Support::ISerializable
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor for a BehaviorModel accepting a name.
                 *  @param name The name to use for this new BehaviorModel.
                 */
                CBehaviorModel(const std::string &name);

                //! Standard destructor.
                ~CBehaviorModel(void);

                /**
                 *  @brief Sets the name of this BehaviorModel.
                 *  @param name The new name to use.
                 */
                void setName(const std::string &name);

                /**
                 *  @brief Returns the name of this BehaviorModel.
                 *  @return The name of this BehaviorModel.
                 */
                const std::string &getName(void);

                /**
                 *
                 */
                virtual void packData(Support::CBitStream &out);
                virtual void unpackData(Support::CBitStream &in);

            // Private Members
            private:
                //! The current name of this BehaviorModel.
                std::string mName;
                //! A set of ComponentBase derivatives attached to this BehaviorModel.
                std::set<IComponent*> mComponents;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_CBEHAVIORMODEL_HPP_
