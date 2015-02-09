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

#include <string>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace CES
    {
        class Entity;

        class ComponentBase
        {
            // Public Methods
            public:
                //! Standard destructor that calls ComponentBase::deinitialize before freeing the memory.
                ~ComponentBase(void) { this->deinitialize(); }

                virtual void initialize(void) = 0;
                virtual void deinitialize(void) = 0;
                virtual void tick(const Kiaro::Common::F32 &DeltaTimeSeconds) { }

            // Private Members
            private:
                //! A pointer to an Entity that this Component belongs to.
                Entity *Parent;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTBASE_H_
