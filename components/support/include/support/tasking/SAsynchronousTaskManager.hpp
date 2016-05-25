/**
 *  @file SAsynchronousTaskManager.hpp
 *  @brief Include file declaring the Support::Tasking::SAsynchronousTaskManager singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_
#define _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_

#include <support/tasking/ITask.hpp>

#include <support/types.hpp>
#include <support/ISingleton.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            typedef struct
            {
                Support::Thread* mThread;
                Tasking::ITask* mTask;
                Support::Atomic<bool> mIsComplete;

            } WorkerContext;

            /**
             *  @brief A asynchrnous task manager singleton that allows for the execution of ITask
             *  derivatives in the context of their own thread by assigning the task context to an available
             *  worker thread. If none are available, the task is stowed until a worker becomes available.
             */
            class SAsynchronousTaskManager : public Support::ISingleton<SAsynchronousTaskManager>
            {
                // Public Methods
                public:
                    void tick(void);

                    /**
                     *  @brief Adds a new task to the asynchronous task manager for execution.
                     *  @param task A pointer to the task.
                     *  @return True if the task was successfully added for asynchronous processing. False if
                     *  it was delegated to the synchronous task manager because the asynchronous task manager
                     *  was created with no workers.
                     *  @throw std::runtime_error Thrown when a NULL task was attempted to be added.
                     */
                    bool addTask(Support::Tasking::ITask* task);
                    bool removeTask(Support::Tasking::ITask* task);

                    const size_t getIdleWorkerCount(void);
                    const size_t getWorkerPoolSize(void);

                    //! Parameter-less constructor.
                    SAsynchronousTaskManager(void);
                    //! Standard destructor.
                    ~SAsynchronousTaskManager(void);

                // Private Members
                private:
                    //! The number of workers this asynchronous task manager is handling.
                    const Common::U8 mPoolSize;

                    //! A set of tasks that were not handed off to a worker yet.
                    Support::Queue<Support::Tasking::ITask*> mScheduledTasks;

                    //! A set of workers that are not doing anything.
                    Support::UnorderedSet<WorkerContext*> mIdleWorkers;

                    //! A set of workers that are doing some thing.
                    Support::UnorderedSet<WorkerContext*> mActiveWorkers;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_SASYNCHRONOUSTASKMANAGER_HPP_
