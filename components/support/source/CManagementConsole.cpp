/**
 *  @file CManagementConsole.cpp
 *  @brief Source file implementing the CManagementConsole class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <support/CManagementConsole.hpp>

namespace Kiaro
{
    namespace Support
    {
        CManagementConsole::CManagementConsole(void)
        {
        }

        CManagementConsole::~CManagementConsole(void)
        {
            for (auto entry : mCallmap)
                delete entry.second;
        }

        //! A regular expression representing valid call signatures.
        static const Support::Regex mCallSignature("[A-z]( +[A-z]+)*", Support::RegexConstants::Extended);

        bool CManagementConsole::eval(const Support::String& input)
        {
            bool success = true;

            // Loop for commands
            std::stringstream commandStream(input);
            Support::String commandString;

            while (std::getline(commandStream, commandString, ';'))
                if (commandString.length() > 0)
                {
                    // Loop for command components
                    std::stringstream componentStream(commandString);
                    Support::String componentString;
                    Support::Vector<Support::String> params;

                    while (std::getline(componentStream, componentString, ' '))
                        if (componentString.length() > 0)
                            params.push_back(componentString);

                    // Evaluate
                    Support::String commandName = params[0];
                    params.erase(params.begin());

                    // Lookup the command
                    auto it = mCallmap.find(commandName);

                    if (it != mCallmap.end())
                    {
                        CONSOLE_DEBUGF("Calling command: %s", commandName.data());
                        (*it).second->invoke(params);
                    }
                    else
                    {
                        CONSOLE_ERRORF("Unknown command: %s", commandName.data());
                        success = false;
                    }
                }

            return success;
        }

        void CManagementConsole::registerFunction(const Support::String& name, ManagementFunction::StoredDelegateType* called)
        {
            // If we're given nullptr, we're removing a function
            if (!called)
            {
                auto it = mCallmap.find(name);

                if (it != mCallmap.end())
                    mCallmap.erase(it);

                return;
            }

            mCallmap[name] = called;
        }

        void CManagementConsole::registerFunction(const Support::String& name, ManagementFunction::StaticDelegateFuncPtr staticPointer)
        {
            this->registerFunction(name, new ManagementFunction::StaticDelegateType(staticPointer));
        }

        void CManagementConsole::registerFunction(const Support::String& name, std::function<void(const Support::Vector<Support::String>&)> lambda)
        {
            this->registerFunction(name, new ManagementFunction::FunctionDelegateType(lambda));
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
