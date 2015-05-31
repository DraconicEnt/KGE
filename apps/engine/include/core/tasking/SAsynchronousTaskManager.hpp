/**
 */

#ifndef _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_
#define _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_

#include <core/tasking/CTask.hpp>

#include <support/support.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            typedef struct
            {
                Kiaro::Support::Thread *mThread;
                Kiaro::Core::Tasking::CTask *mTask;
                Kiaro::Support::Atomic<bool> mIsComplete;

            } WorkerContext;

            class SAsynchronousTaskManager
            {
                // Public Methods
                public:
                    static SAsynchronousTaskManager *getPointer(const size_t &poolSize = WORKER_THREAD_POOL_SIZE);
                    static void destroy(void);

                    void tick(void);

                    bool addTask(Kiaro::Core::Tasking::CTask *task);
                    bool removeTask(Kiaro::Core::Tasking::CTask *task);

                    const size_t getIdleWorkerCount(void);
                    const size_t &getWorkerPoolSize(void);

                // Private Methods
                private:
                    SAsynchronousTaskManager(const size_t &poolSize);
                    ~SAsynchronousTaskManager(void);

                // Private Members
                private:
                    const size_t mPoolSize;

                    //! A set of tasks that were not handed off to a worker yet.
                    Kiaro::Support::Queue<Kiaro::Core::Tasking::CTask *> mScheduledTasks;

                    //! A set of workers that are not doing anything.
                    Kiaro::Support::UnorderedSet<WorkerContext *> mIdleWorkers;

                    //! A set of workers that are doing some thing.
                    Kiaro::Support::UnorderedSet<WorkerContext *> mActiveWorkers;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_
