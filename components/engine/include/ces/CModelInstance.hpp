/**
 *  @file CModelInstance.hpp
 *  @brief Include file declaring the CBuoyancy component.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CES_CMODELINSTANCE_HPP_
#define _INCLUDE_CES_CMODELINSTANCE_HPP_

#include <ces/CComponentRoot.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            //!
            class CModelInstance
            {
                private:
                    IComponent* mRoot;

                public:
                    CModelInstance(CComponentRoot* root);

                    void update(void);

                    void setModel(IComponent* root);

                    IComponent* getModel(void);
            };
        }
    }
}
#endif // _INCLUDE_CES_CMODELINSTANCE_HPP_
