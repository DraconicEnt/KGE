/**
 *  @file CModelInstance.cpp
 *  @brief Source file containing programming for the CModelInstance class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CModelInstance.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            CModelInstance::CModelInstance(CComponentRoot* root) : mRoot(root)
            {
                // TODO: When we instantiate, we create the renderables in mRoot
            }

            void CModelInstance::update(void)
            {
                mRoot->evaluate(this);
            }

            void CModelInstance::setModel(IComponent* root)
            {
                // TODO: When we change the model, we delete/modify the renderables in mRoot
                mRoot = root;
            }

            IComponent* CModelInstance::getModel(void)
            {
                return mRoot;
            }
        }
    }
}
