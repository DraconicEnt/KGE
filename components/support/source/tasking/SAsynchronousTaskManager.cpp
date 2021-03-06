/**
 *  @file SAsynchronousTasKmanager.cpp
 *  @brief Source file implementing the SAsynchronousTaskManager singleton class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <chrono>
#include <thread>

#include <support/Console.hpp>
#include <support/SSettingsRegistry.hpp>

#include <support/tasking/SSynchronousTaskManager.hpp>
#include <support/tasking/SAsynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static void workerThreadLogic(WorkerContext* context)
            {
                assert(context);

                // Keep running, wait for tasks
                while (true)
                {
                    if (context->mIsComplete || !context->mTask)
                    {
                        // Nothing to do, just sleep for a moment before restarting the loop.
                        std::this_thread::sleep_for(std::chrono::milliseconds(32));
                        continue;
                    }

                    context->mIsComplete = context->mTask->tick(0.00f);
                }
            }

            SAsynchronousTaskManager::SAsynchronousTaskManager(void) : mPoolSize(Support::SSettingsRegistry::getInstance()->getValue<Common::U8>("System::WorkerThreadCount"))
            {
                if (mPoolSize == 0)
                {
                    CONSOLE_INFO("Using no asynchrnous workers; will delegate to the synchronous task manager.");
                    return;
                }

                for (Common::U8 iteration = 0; iteration < mPoolSize; ++iteration)
                {
                    WorkerContext* currentWorker = new WorkerContext();
                    currentWorker->mTask = nullptr;
                    currentWorker->mIsComplete = true;
                    currentWorker->mThread = new Support::Thread(workerThreadLogic, currentWorker);
                    currentWorker->mThread->detach();

                    mIdleWorkers.insert(mIdleWorkers.end(), currentWorker);
                }

                CONSOLE_INFOF("Initialized with %u workers.", mPoolSize);
            }

            SAsynchronousTaskManager::~SAsynchronousTaskManager(void)
            {
                // Kill any active workers
                for (WorkerContext* worker : mActiveWorkers)
                {
                    delete worker;
                }

                // Kill inactive workers
                for (WorkerContext* worker : mIdleWorkers)
                {
                    delete worker;
                }
            }

            void SAsynchronousTaskManager::tick(void)
            {
                // If we've got any scheduled tasks, hand it off to some idle worker
                for (size_t iteration = 0; iteration < mScheduledTasks.size() && iteration < mIdleWorkers.size(); ++iteration)
                {
                    // TODO (Robert MacGregor#9): More efficiency?
                    WorkerContext* currentWorker = *mIdleWorkers.begin();
                    mIdleWorkers.erase(currentWorker);
                    ITask* currentTask = mScheduledTasks.front();
                    mScheduledTasks.pop();
                    currentWorker->mTask = currentTask;
                    currentWorker->mIsComplete = false;
                    mActiveWorkers.insert(mActiveWorkers.end(), currentWorker);
                }

                // Do we have any tasks that have completed?
                for (WorkerContext* currentWorker : mActiveWorkers)
                {
                    if (currentWorker->mIsComplete)
                    {
                        // TODO (Robert MacGregor#9): Task completion notice?
                        delete currentWorker->mTask;
                        currentWorker->mTask = nullptr;
                        mActiveWorkers.erase(currentWorker);
                        mIdleWorkers.insert(mIdleWorkers.end(), currentWorker);
                    }
                }
            }

            size_t SAsynchronousTaskManager::getIdleWorkerCount(void)
            {
                return mIdleWorkers.size();
            }

            size_t SAsynchronousTaskManager::getWorkerPoolSize(void)
            {
                return mPoolSize;
            }

            bool SAsynchronousTaskManager::addTask(ITask* task)
            {
                if (!task)
                {
                    throw std::runtime_error("SAsynchronousTaskManager: Cannot add a NULL task.");
                }

                // Config demands that we don't do anything asynchronously, so we delegate to the synchronous tasker
                if (mPoolSize == 0)
                {
                    SSynchronousTaskManager::getInstance()->addTask(task);
                    return false;
                }

                mScheduledTasks.push(task);
                return true;
            }

            bool SAsynchronousTaskManager::removeTask(ITask* task)
            {
                if (!task)
                {
                    throw std::runtime_error("SAsynchronousTaskManager: Cannot remove a NULL task.");
                }

                for (WorkerContext* currentWorker: mActiveWorkers)
                {
                    if (currentWorker->mTask == task)
                    {
                        currentWorker->mIsComplete = true;
                        currentWorker->mTask = nullptr;
                        return true;
                    }
                }

                return false;
            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
