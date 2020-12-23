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

#include <regex>

#include <support/String.hpp>
#include <support/Vector.hpp>
#include <support/UnorderedMap.hpp>

#include <easydelegate/easydelegate.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief The CManagementConsole provides a simple method of interacting with the engine without use of a sophisticated scripting engine. This
         *  interface is intended for quick inquiries and administrative changes of a running game server with the management console enabled.
         */
        class CManagementConsole
        {
            // Public Members
            public:
                //! A delegate representing a callable management function.
                typedef EasyDelegate::DelegateSet<void, const Support::Vector<Support::String>& > ManagementFunction;

            // Private Members
            private:
                //! A mapping of management function names to their representing delegates.
                Support::UnorderedMap<Support::String, ManagementFunction::StoredDelegateType*> mCallmap;

            // Public Methods
            public:
                //! Parameter-less constructor.
                CManagementConsole(void);
                //! Standard destructor.
                ~CManagementConsole(void);

                /**
                 *  @brief Evaluates an input string for commands.
                 *  @param input The input string to process.
                 *  @return True for execution success. False otherwise.
                 */
                bool eval(const Support::String& input);

                /**
                 *  @brief Registers a callable management function to the CManagementConsole.
                 *  @param name The name of the management function.
                 *  @param called A pointer to the delegate to invoke representing this management function.
                 */
                void registerFunction(const Support::String& name, ManagementFunction::StoredDelegateType* called);

                /**
                 *  @brief Registers a callable management function to the CManagementConsole.
                 *  @param name The name of the management function.
                 *  @param staticPointer A pointer to the static method to call representing this management function.
                 */
                void registerFunction(const Support::String& name, ManagementFunction::StaticDelegateFuncPtr staticPointer);

                void registerFunction(const Support::String& name, std::function<void(const Support::Vector<Support::String>&)> lambda);

                /**
                 *  @brief Registers a callable management function to the CManagementConsole using a member method pointer.
                 *  @param name The name of the management function.
                 *  @param thisPointer A pointer to the instance this method should be invoked against.
                 *  @param memberPointer A pointer to the member method to call representing this management function.
                 */
                template <typename className>
                void registerFunction(const Support::String& name, className* thisPointer, ManagementFunction::MemberDelegateFuncPtr<className> memberPointer)
                {
                    this->registerFunction(name, new ManagementFunction::MemberDelegateType<className>(memberPointer, thisPointer));
                }
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_MANAGEMENTCONSOLE_HPP_
