/**
 *  @file SSignalHandler.cpp
 *  @brief Singleton for handling crashes and generating reports.
 */

#include <signal.h>
#include <unistd.h>

#include <support/SSignalHandler.hpp>

namespace Kiaro
{
    namespace Support
    {
        static void handleProcessCrash(int signum)
        {
            SSignalHandler* handler = SSignalHandler::getPointer();
            if (handler->mSignalHandlers.find(SSignalHandler::SignalType::Crash) != handler->mSignalHandlers.end())
                handler->mSignalHandlers[SSignalHandler::SignalType::Crash]->invoke();

            signal(SIGBUS, SIG_DFL);
            signal(SIGSEGV, SIG_DFL);

            kill(getpid(), signum);
        }

        static void handleProcessTermination(int signal)
        {
            SSignalHandler* handler = SSignalHandler::getPointer();
            if (handler->mSignalHandlers.find(SSignalHandler::SignalType::Termination) != handler->mSignalHandlers.end())
                handler->mSignalHandlers[SSignalHandler::SignalType::Termination]->invoke();
        }

        static void handleCPUOverload(int signal)
        {
            SSignalHandler* handler = SSignalHandler::getPointer();
            if (handler->mSignalHandlers.find(SSignalHandler::SignalType::CPUUsage) != handler->mSignalHandlers.end())
                handler->mSignalHandlers[SSignalHandler::SignalType::CPUUsage]->invoke();
        }

        SSignalHandler::SSignalHandler(void)
        {
            signal(SIGINT, handleProcessTermination);
            signal(SIGTERM, handleProcessTermination);

            signal(SIGXCPU, handleCPUOverload);

            signal(SIGBUS, handleProcessCrash);
            signal(SIGSEGV, handleProcessCrash);
        }
    }
}
