/**
 *  @file ComponentBase.h
 *  @brief Include file defining the Kiaro::CES::ComponentBase class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2016 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_COMPONENTBASE_H_
#define _INCLUDE_KIARO_CES_COMPONENTBASE_H_

#include <set>
#include <string>

#include <support/common.hpp>

namespace Kiaro
{
    namespace CES
    {
        class CBehaviorModel;

        //! An enumeration representing the individual component types.
        enum COMPONENT_NAME
        {
            //! NULL component; shouldn't happen.
            COMPONENT_NULL = 0,
            //! Gravity component.
            COMPONENT_GRAVITY = 1,
        };

        /**
         *  @brief Base interface type for all behavior model components.
         */
        class IComponent
        {
                // Protected Members
            protected:
                COMPONENT_NAME mComponentTypeIdentifier;

                // Private Members
            private:
                //! A set of BehaviorModel instances that this IComponent belongs to.
                std::set<CBehaviorModel*> mBehaviorModels;

                // Public Methods
            public:
                //! Standard destructor.
                ~IComponent(void)
                {
                    this->deinitialize();
                }

                /**
                 *  @brief Initializes the behavior model component
                 *  with a given entity that uses behavior models. It
                 *  basically performs all of the necessary logic to "bind"
                 *  the component's effect to a given entity.
                 */
                virtual void initialize(void) = 0;

                /**
                 *  @brief Deinitializes the behavior model component with a given
                 *  entity that it has been previously initialized with.
                 */
                virtual void deinitialize(void) = 0;

                /**
                 *  @brief Ticks the behavior model component logic by an arbitrary
                 *  increment of seconds.
                 *  @param deltaTimeSeconds The time in seconds that has passed for the
                 *  ticking to fill in.
                 */
                virtual void tick(const Common::F32& deltaTimeSeconds) { }
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTBASE_H_
