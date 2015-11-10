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

#include <support/tasking/ITask.hpp>

#include <support/support.hpp>

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
            class SAsynchronousTaskManager
            {
                // Public Methods
                public:
                    /**
                     *  @brief Returns a pointer to the asynchrnous task manager instance, allocating a new
                     *  one if necessary.
                     *  @return A pointer to the current asynchrnous task manager instance.
                     */
                    static SAsynchronousTaskManager* getPointer(void);
                    //! Destroys the current asynchronous task manager instance if there is one.
                    static void destroy(void);

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
                    const size_t& getWorkerPoolSize(void);

                // Private Methods
                private:
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
