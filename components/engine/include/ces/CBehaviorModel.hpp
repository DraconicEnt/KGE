/**
 *  @file CBehaviorModel.hpp
 *  @brief Include file defining the Kiaro::CES::CBehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_
#define _INCLUDE_KIARO_CES_BEHAVIORMODEL_HPP_

#include <support/support.hpp>

#include <support/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace CES
    {
        class IComponent;

        /**
         *  @brief A behavior model represents the overall behavior of some object in the game world
         *  in a highly dynamic, reprogrammable fashion while also maintaining the ability to be
         *  networked to arbitrary clients for clientside simulation.
         */
        class CBehaviorModel : public Support::ISerializable
        {
                // Public Methods
            public:
                /**
                 *  @brief Constructor for a BehaviorModel accepting a name.
                 *  @param name The name to use for this new BehaviorModel.
                 */
                CBehaviorModel(const Support::String& name);

                //! Standard destructor.
                ~CBehaviorModel(void);

                /**
                 *  @brief Sets the name of this BehaviorModel.
                 *  @param name The new name to use.
                 */
                void setName(const Support::String& name);

                /**
                 *  @brief Returns the name of this BehaviorModel.
                 *  @return The name of this BehaviorModel.
                 */
                const Support::String& getName(void);

                /**
                 *
                 */
                virtual void packData(Support::CBitStream& out);
                virtual void unpackData(Support::CBitStream& in);

                // Private Members
            private:
                //! The current name of this BehaviorModel.
                Support::String mName;
                //! A set of ComponentBase derivatives attached to this BehaviorModel.
                Support::Set<IComponent*> mComponents;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_CBEHAVIORMODEL_HPP_
