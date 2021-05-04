/**
 *  @file SCrashHandler.hpp
 *  @brief Singleton for handling crashes and generating reports.
 */

#ifndef _INCLUDE_SUPPORT_SCRASHHANDLER_HPP_
#define _INCLUDE_SUPPORT_SCRASHHANDLER_HPP_

#include <easydelegate/easydelegate.hpp>
#include <support/types.hpp>
#include <support/UnorderedMap.hpp>
#include <support/ISingleton.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief A singleton for handling operating system signals.
         *  @detail Technically, we should use as little code as possible in our signal handlers because the program state is unknown in cases of 'crash' signal
         *  types, but unless some runaway code managed to stomp literally all over program memory, then this extra overhead *shouldn't* be a problem when they
         *  are thrown.
         */
        class SSignalHandler : public ISingleton<SSignalHandler>
        {
            //! Public Members
            public:
                //! Generalized signal types that the library can process.
                enum SignalType
                {
                    //! A generalized signal for when the program has crashed. On Unix systems, this can be triggered by SIGBUS or SIGSEGV.
                    Crash,
                    //! A generalized signal for when the program is being terminated.
                    Termination,
                    //! A generalized signal for when too much CPU resources are being used. On Linux, this is called when SIGXCPU arrives.
                    CPUUsage,
                };

                //! The callable type to use for handling signals.
                typedef EasyDelegate::DelegateSet<void> SignalHandlerType;

                //! Handlers to use for incoming signals.
                Support::UnorderedMap<Common::U32, SignalHandlerType::StoredDelegateType*> mSignalHandlers;

            //! Public Methods
            public:
                SSignalHandler(void);
        };
    }
}

#endif // _INCLUDE_SUPPORT_SCRASHHANDLER_HPP_
