/**
 *  @file CManagementConsole.hpp
 *  @brief Include file declaring the CManagementConsole and its class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_MANAGEMENTCONSOLE_HPP_
#define _INCLUDE_SUPPORT_MANAGEMENTCONSOLE_HPP_

#include <support/Regex.hpp>
#include <support/String.hpp>
#include <support/Vector.hpp>
#include <support/UnorderedMap.hpp>

#include <easydelegate/easydelegate.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CManagementConsole
        {
                // Public Members
            public:
                typedef EasyDelegate::DelegateSet<void, const Support::Vector<Support::String>& > ManagementFunction;

                // Private Members
            private:
                Support::UnorderedMap<Support::String, ManagementFunction::StoredDelegateType*> mCallmap;


                // Public Methods
            public:
                CManagementConsole(void);
                ~CManagementConsole(void);

                bool eval(const Support::String& input);

                void registerFunction(const Support::String& name, ManagementFunction::StoredDelegateType* called);

                void registerFunction(const Support::String& name, ManagementFunction::StaticDelegateFuncPtr staticPointer);

                template <typename className>
                void registerFunction(const Support::String& name, className* thisPointer, ManagementFunction::MemberDelegateFuncPtr<className> memberPointer)
                {
                    this->registerFunction(name, new ManagementFunction::MemberDelegateType<className>(memberPointer, thisPointer));
                }
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_MANAGEMENTCONSOLE_HPP_
