/**
 */

#ifndef _INCLUDE_CTASK_HPP
#define _INCLUDE_CTASK_HPP

#include <support/support.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            class CTask
            {
                // Public Methods
                public:

                    virtual void initialize(void) = 0;

                    virtual bool tick(const Common::F32 &deltaTime) = 0;

                    virtual void deinitialize(void) = 0;

                // Public Members
                public:
                    bool mIsComplete;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro

#endif // _INCLUDE_CTASK_HPP
