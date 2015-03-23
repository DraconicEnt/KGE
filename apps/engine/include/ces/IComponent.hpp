/**
 *  @file ComponentBase.h
 *  @brief Include file defining the Kiaro::CES::ComponentBase class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_COMPONENTBASE_H_
#define _INCLUDE_KIARO_CES_COMPONENTBASE_H_

#include <set>
#include <string>

#include <engine/Common.hpp>

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

        class IComponent
        {
            // Public Methods
            public:
                //! Standard destructor that calls ComponentBase::deinitialize before freeing the memory.
                ~IComponent(void) { this->deinitialize(); }

                virtual void initialize(void) = 0;
                virtual void deinitialize(void) = 0;
                virtual void tick(const Kiaro::Common::F32 &DeltaTimeSeconds) { }

            // Protected Members
            protected:
                COMPONENT_NAME mComponentTypeIdentifier;

            // Private Members
            private:
                //! A set of BehaviorModel instances that this ComponentBase belongs to.
                std::set<CBehaviorModel*> mBehaviorModels;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTBASE_H_
