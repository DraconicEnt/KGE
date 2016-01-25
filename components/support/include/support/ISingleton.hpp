/**
 *  @file ISingleton.hpp
 */ 

#ifndef _INCLUDE_SUPPORT_ISINGLETON_HPP_
#define _INCLUDE_SUPPORT_ISINGLETON_HPP_

#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Support
    {
        template <typename child>
        class ISingleton
        {
            // Private Members
            private:
                static child* mInstance;

            public:
                template <typename... parameters>
                static child* getPointer(parameters... params)
                {
                    if (!mInstance)
                        mInstance = new child(params...);
                        
                    return mInstance;
                }
                
                static void destroy(void)
                {
                    if (mInstance)
                    {
                        delete mInstance;
                        mInstance = nullptr;
                    }
                }
        };
        
        template <typename child>
        child* ISingleton<child>::mInstance = nullptr;
    }
}
#endif // _INCLUDE_SUPPORT_ISINGLETON_HPP_
