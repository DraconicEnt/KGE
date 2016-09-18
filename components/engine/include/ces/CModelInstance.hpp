#ifndef _INCLUDE_CES_CMODELINSTANCE_HPP_
#define _INCLUDE_CES_CMODELINSTANCE_HPP_

#include <ces/CComponentRoot.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
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
