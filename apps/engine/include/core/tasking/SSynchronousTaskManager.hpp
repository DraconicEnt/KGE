/**
 */

#ifndef _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
#define _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_

#include <core/common.hpp>

#include <core/tasking/CTask.hpp>
#include <support/support.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            class SSynchronousTaskManager
            {
                // Public Methods
                public:
                    static SSynchronousTaskManager *getPointer(void);
                    static void destroy(void);

                    void tick(const Kiaro::Common::F32 &deltaTime);
                    bool addTask(Kiaro::Core::Tasking::CTask *task);
                    bool removeTask(Kiaro::Core::Tasking::CTask *task);

                // Private Methods
                private:
                    SSynchronousTaskManager(void);
                    ~SSynchronousTaskManager(void);

                // Private Members
                private:
                    Kiaro::Support::UnorderedSet<Kiaro::Core::Tasking::CTask *> mTaskList;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro

#endif // _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
