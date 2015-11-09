/**
 *  @file SAsynchronousTaskManager.hpp
 *  @brief Include file declaring the Support::Tasking::SAsynchrnousTaskManager singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
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
                Support::Thread *mThread;
                Core::Tasking::ITask *mTask;
                Support::Atomic<bool> mIsComplete;

            } WorkerContext;

            class SAsynchronousTaskManager
            {
                // Public Methods
                public:
                    static SAsynchronousTaskManager *getPointer(void);
                    static void destroy(void);

                    void tick(void);

                    bool addTask(Core::Tasking::ITask *task);
                    bool removeTask(Core::Tasking::ITask *task);

                    const size_t getIdleWorkerCount(void);
                    const size_t& getWorkerPoolSize(void);

                // Private Methods
                private:
                    SAsynchronousTaskManager(void);
                    ~SAsynchronousTaskManager(void);

                // Private Members
                private:
                    const Common::U8 mPoolSize;

                    //! A set of tasks that were not handed off to a worker yet.
                    Support::Queue<Kiaro::Core::Tasking::ITask*> mScheduledTasks;

                    //! A set of workers that are not doing anything.
                    Support::UnorderedSet<WorkerContext*> mIdleWorkers;

                    //! A set of workers that are doing some thing.
                    Support::UnorderedSet<WorkerContext*> mActiveWorkers;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_
