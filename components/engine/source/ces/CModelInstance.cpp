/**
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
