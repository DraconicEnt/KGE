/**
 */

#ifndef _INCLUDE_CVIRTUALMOUNT_HPP_
#define _INCLUDE_CVIRTUALMOUNT_HPP_

#include <support/types.hpp>
#include <support/common.hpp>

#include <ces/IComponent.hpp>


namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class CVirtualMount : public IComponent
            {
                public:
                    virtual COMPONENT_TYPE getComponentType(void);

                    //! If <0, no mount is used and we simply operate from model center. Otherwise, we operate relative to the transformation of the specified mount.
                    int mMountID;

                    //! Offset from origin.
                    Support::Vector3DF mTranslation;

                    //! Euclideon angle for now.
                    Support::Vector3DF mRotation;
            };
        }
    }
}
#endif // _INCLUDE_CVIRTUALMOUNT_HPP_
