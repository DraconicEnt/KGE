/**
 *  @file CBuoyancy.hpp
 *  @brief Include file declaring the CBuoyancy component.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CBUOYANCY_HPP_
#define _INCLUDE_CBUOYANCY_HPP_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            //! A pluggable component representing object buoyancy.
            class CBuoyancy : public IComponent
            {
                private:
                    //! The amount of buoyancy to use.
                    Common::F32 mBuoyancy;

                public:
                    /**
                     *  @brief Constructor accepting a buoyancy value.
                     *  @param buoyancy The buoyancy value to use.
                     */
                    CBuoyancy(Common::F32 buoyancy);

                    /**
                     *  @brief Handler method for attaching components.
                     *  @param component The component to attach.
                     */
                    virtual void attachComponent(IComponent* component);

                    /**
                     *  @brief Determines the type of component.
                     *  @return The type of component this is.
                     */
                    virtual COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CBUOYANCY_HPP_
