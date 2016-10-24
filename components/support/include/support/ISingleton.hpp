/**
 *  @file ISingleton.hpp
 *  @brief Include file declaring the ISingleton singleton interface class type.
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
        template <typename childClass>
        class ISingleton
        {
            // Private Members
            private:
                /**
                 *  @brief An inner class instantiation wrapper to allow us to construct singleton classes despite their constructors
                 *  and destructors being marked as protected.
                 */
                class InstantiationWrapper : public childClass
                {
                    // Public Methods
                    public:
                        friend class ISingleton<childClass>;

                        /**
                         *  @brief Constructor accepting any parameters and passing them on to the child class instantiation.
                         */
                        template <typename... parameters>
                        InstantiationWrapper(parameters... params) : childClass(params...)
                        {

                        }
                };

                //! The pointer to the stored singleton instance.
                static InstantiationWrapper* sInstance;

            // Public Methods
            public:
                /**
                 *  @brief Returns the pointer to the singleton instance, creating a new
                 *  instance if necessary.
                 *  @return A pointer to the singleton instance.
                 */
                template <typename... parameters>
                static childClass* getInstance(parameters... params)
                {
                    if (!sInstance)
                        sInstance = new InstantiationWrapper(params...);

                    return sInstance;
                }

                /**
                 *  @brief Returns the pointer to the singleton instance with no attempted instantiation
                 *  if there is no instance already.
                 *  @return A pointer to the singleton instance.
                 *  @retval nullptr Returned when the singleton has not been initialized.
                 */
                static childClass* getPointer(void)
                {
                    return sInstance;
                }

                /**
                 *  @brief Destroys the singleton instance, causing the next call to getPointer
                 *  to create a new instance.
                 */
                static void destroy(void)
                {
                    delete sInstance;
                    sInstance = nullptr;
                }
        };

        template <typename childClass>
        typename ISingleton<childClass>::InstantiationWrapper* ISingleton<childClass>::sInstance = nullptr;
    }
}
#endif // _INCLUDE_SUPPORT_ISINGLETON_HPP_
