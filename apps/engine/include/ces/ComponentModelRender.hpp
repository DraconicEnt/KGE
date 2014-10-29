/**
 *  @file ComponentModelRender.h
 *  @brief Include file defining the Kiaro::CES::ComponentModelRender class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_COMPONENTMODELRENDER_H_
#define _INCLUDE_KIARO_CES_COMPONENTMODELRENDER_H_

#include <support/String.hpp>

#include "ComponentBase.hpp"

namespace Kiaro
{
    namespace CES
    {
        class Entity;

        class ComponentModelRender : public ComponentBase
        {
            public:
            ComponentModelRender(Entity *parent);
            void Initialize(void);
            void Deinitialize(void);

            private:
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTMODELRENDER_H_
