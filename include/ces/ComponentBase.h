/**
 *  @file ComponentBase.h
 *  @brief Include file defining the Kiaro::CES::ComponentBase class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_COMPONENTBASE_H_
#define _INCLUDE_KIARO_CES_COMPONENTBASE_H_

#include <support/String.h>

namespace Kiaro
{
    namespace CES
    {
        class Entity;

        class ComponentBase
        {
            public:
            ComponentBase(Entity *parent) { this->Parent = parent; }
            ~ComponentBase(void) { this->Deinitialize(); }

            virtual void Initialize(void) = 0;
            virtual void Deinitialize(void) = 0;
            virtual void Tick(Kiaro::Common::F32 DeltaTimeSeconds) { }

            private:
            //! A pointer to an Entity that this Component belongs to.
            Entity *Parent;
        };
    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_COMPONENTBASE_H_
