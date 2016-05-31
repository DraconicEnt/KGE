/**
 *  @file IComponent.h
 *  @brief Include file defining the Kiaro::CES::CComponentGravity class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2016 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_CCOMPONENTGRAVITY_HPP_
#define _INCLUDE_KIARO_CES_CCOMPONENTGRAVITY_HPP_

#include <string>

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace CES
    {
        class Entity;

        /**
         *  @brief A gravity component to be attached to a behavior model which enables the pull
         *  of gravity against any entity using the behavior model it is attached to.
         */
        class CComponentGravity : public IComponent
        {
                // Public Methods
            public:
                CComponentGravity(void);

                virtual void initialize(void);
                virtual void deinitialize(void);

                // Private Members
            private:
                //! A pointer to an Entity that this Component belongs to.
                Entity* Parent;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_CCOMPONENTGRAVITY_HPP_
