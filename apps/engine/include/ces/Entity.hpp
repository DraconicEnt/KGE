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

#include <set>

#include <support/String.hpp>

namespace Kiaro
{
    namespace CES
    {
        class ComponentBase;

        class Entity
        {
            public:
            /**
             *  @brief Constructor accepting a name.
             *  @param name A pointer to a Kiaro::Common::C8 that is to be
             *  the name of the model, if this is a model. If this is NULL
             *  or 0x00, then this is assumed to not be a model.
             */
            Entity(Kiaro::Common::C8 *name = 0x00);

            public:

            std::set<ComponentBase*> mComponents;
            private:

            //! The name of the model, if it is a model.
            Kiaro::Common::C8 *mName;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTMODELRENDER_H_
