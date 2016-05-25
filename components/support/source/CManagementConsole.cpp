/**
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

        }

        static const Support::Regex mCallSignature("[A-z]( +[A-z]+)*", Support::RegexConstants::Extended);

        bool CManagementConsole::eval(const Support::String& input)
        {
            // FIXME: Support multi-line calls with string & numeric params
            Support::Vector<Support::String> params;

            std::stringstream ss(input);
            Support::String item;
            while (std::getline(ss, item, ' '))
                if (item.length() > 0)
                    params.push_back(item);

            Support::String command = params[0];
            params.erase(params.begin());

            // Lookup the command
            auto it = mCallmap.find(command);

            if (it != mCallmap.end())
            {
                (*it).second->invoke(params);
                return true;
            }

            CONSOLE_ERRORF("Unknown command: %s", command.data());
            return false;
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
    } // End NameSpace Support
} // End NameSpace Kiaro
