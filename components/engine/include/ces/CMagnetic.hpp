/**
 *  @file CMagnetic.hpp
 *  @brief Include file declaring the CMagntic pluggable component.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CES_CMAGNETIC_
#define _INCLUDE_CES_CMAGNETIC_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            //! Class representing a magnetic property.
            class CMagnetic : public IComponent
            {
                private:
                    //! The magnetism to use.
                    Common::F32 mMagnetism;

                public:
                    /**
                     *  @brief Constructor accepting a magnetism strength.
                     *  @param strength The magnetic strength to use.
                     */
                    CMagnetic(Common::F32 strength);

                    /**
                     *  @brief Handler method for attaching components.
                     *  @param component The component to attach.
                     */
                    virtual void attachComponent(IComponent* component);

                    /**
                     *  @brief Determines the type of component.
                     *  @return The type of component this is.
                     */
                    COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CES_CMAGNETIC_
