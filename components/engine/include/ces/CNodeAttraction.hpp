/**
 */

#ifndef _INCLUDE_CNODEATTRACTION_HPP_
#define _INCLUDE_CNODEATTRACTION_HPP_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class CNodeAttraction : public IComponent
            {
                public:
                    //! The strength of the attraction.
                    Common::F32 mStrength;

                    //! The top level index of the renderable to attract towards.
                    Common::S32 mRenderableIndex;

                public:
                    virtual COMPONENT_TYPE getComponentType(void);

                    virtual void attachComponent(IComponent* component);

                    virtual bool evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Set<IComponent*>& root);
            };
        }
    }
}
#endif // _INCLUDE_CNODEATTRACTION_HPP_
