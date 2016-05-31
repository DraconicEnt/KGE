/**
 *  @file ISingleton.hpp
 *  @brief Include file declaring the ISingleton class type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_ISINGLETON_HPP_
#define _INCLUDE_SUPPORT_ISINGLETON_HPP_

#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief The ISingleton is an interface class for singleton classes to
         *  derive from in order to implement a common singleton API for instantiation
         *  and destruction.
         */
        template <typename child>
        class ISingleton
        {
                // Private Members
            private:
                //! The pointer to the instance
                static child* mInstance;

                // Public Methods
            public:
                /**
                 *  @brief Returns the pointer to the singleton instance, creating a new
                 *  instance if necessary.
                 *  @return A pointer to the singleton instance.
                 */
                template <typename... parameters>
                static child* getPointer(parameters... params)
                {
                    if (!mInstance)
                        mInstance = new child(params...);

                    return mInstance;
                }

                /**
                 *  @brief Destroys the singleton instance, causing the next call to getPointer
                 *  to create a new instance.
                 */
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
