/**
 *  @file BehaviorModel.h
 *  @brief Include file defining the Kiaro::CES::BehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_
#define _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_

#include <string>

#include <engine/Common.hpp>
#include <support/BitStream.hpp>

namespace Kiaro
{
    namespace CES
    {
        class BehaviorModel : public Kiaro::Support::SerializableObjectBase
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor for a BehaviorModel accepting a name.
                 *  @param name The name to use for this new BehaviorModel.
                 */
                BehaviorModel(const std::string &name);

                //! Standard destructor.
                ~BehaviorModel(void);

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
                virtual void packData(Kiaro::Support::BitStream &out);
                virtual void unpackData(Kiaro::Support::BitStream &in);

            // Private Members
            private:
                //! The current name of this BehaviorModel.
                std::string mName;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_
