/**
 */

#include <core/Logging.hpp>
#include <core/SSettingsRegistry.hpp>

#include <core/tasking/SSynchronousTaskManager.hpp>
#include <core/tasking/SAsynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            static SAsynchronousTaskManager* sInstance = NULL;

            static void workerThreadLogic(Core::Tasking::WorkerContext* context)
            {
                // TODO (Robert MacGregor #9): Detect thread error?
                if (!context)
                    return;

                // Keep running, wait for tasks
                while (true)
                {
                    if (context->mIsComplete || !context->mTask)
                    {
                        // Nothing to do, just sleep for a moment before restarting the loop.
                        std::this_thread::sleep_for(std::chrono::milliseconds(WORKER_SLEEP_TIME_MS));
                        continue;
                    }

                    context->mIsComplete = context->mTask->tick(0.00f);
                }
            }

            SAsynchronousTaskManager* SAsynchronousTaskManager::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SAsynchronousTaskManager();

                return sInstance;
            }

            void SAsynchronousTaskManager::destroy(void)
            {
                if (sInstance)
                {
                    delete sInstance;
                    sInstance = NULL;
                }
            }

            void SAsynchronousTaskManager::tick(void)
            {
                // If we've got any scheduled tasks, hand it off to some idle worker
                for (size_t iteration = 0; iteration < mScheduledTasks.size() && iteration < mIdleWorkers.size(); iteration++)
                {
                    // TODO (Robert MacGregor#9): More efficiency?
                    Kiaro::Core::Tasking::WorkerContext *currentWorker = *mIdleWorkers.begin();
                    mIdleWorkers.erase(currentWorker);

                    Kiaro::Core::Tasking::CTask *currentTask = mScheduledTasks.front();
                    mScheduledTasks.pop();

                    currentWorker->mTask = currentTask;
                    currentWorker->mIsComplete = false;

                    mActiveWorkers.insert(mActiveWorkers.end(), currentWorker);
                }

                // Do we have any tasks that have completed?
                for (auto it = mActiveWorkers.begin(); it != mActiveWorkers.end(); it++)
                {
                    Core::Tasking::WorkerContext* currentWorker = *it;

                    if (currentWorker->mIsComplete)
                    {
                        // TODO (Robert MacGregor#9): Task completion notice?
                        delete currentWorker->mTask;
                        currentWorker->mTask = NULL;

                        mActiveWorkers.erase(currentWorker);
                        mIdleWorkers.insert(mIdleWorkers.end(), currentWorker);
                    }
                }
            }

            const size_t SAsynchronousTaskManager::getIdleWorkerCount(void)
            {
                return mIdleWorkers.size();
            }

            const size_t& SAsynchronousTaskManager::getWorkerPoolSize(void)
            {
                return mPoolSize;
            }

            bool SAsynchronousTaskManager::addTask(Core::Tasking::CTask* task)
            {
                if (!task)
                {
                    throw std::runtime_error("SAsynchronousTaskManager: Cannot add a NULL task.");
                    return false;
                }

                // Config demands that we don't do anything asynchronously, so we delegate to the synchronous tasker
                if (mPoolSize == 0)
                {
                    SSynchronousTaskManager::getPointer()->addTask(task);
                    return false;
                }

                mScheduledTasks.push(task);
                return true;
            }

            bool SAsynchronousTaskManager::removeTask(Core::Tasking::CTask *task)
            {
                if (!task)
                {
                    throw std::runtime_error("SAsynchronousTaskManager: Cannot remove a NULL task.");
                    return false;
                }

                // TODO (Robert MacGregor#9): Find and halt the thread safely.

               // return mScheduledTasks.erase(task) != 0;
               return false;
            }

            SAsynchronousTaskManager::SAsynchronousTaskManager(void) : mPoolSize(Core::SSettingsRegistry::getPointer()->getValue<Common::U8>("WorkerThreadCount"))
            {
                if (mPoolSize == 0)
                {
                    Core::Logging::write(Core::Logging::MESSAGE_INFO, "SAsynchronousTaskManager: Using no asynchrnous workers; will delegate to the synchronous task manager.");
                    return;
                }

                for (Common::U8 iteration = 0; iteration < mPoolSize; iteration++)
                {
                    Kiaro::Core::Tasking::WorkerContext* currentWorker = new Kiaro::Core::Tasking::WorkerContext;
                    currentWorker->mTask = NULL;
                    currentWorker->mIsComplete = true;
                    currentWorker->mThread = new Support::Thread(workerThreadLogic, currentWorker);
                    currentWorker->mThread->detach();

                    mIdleWorkers.insert(mIdleWorkers.end(), currentWorker);
                }

                Core::Logging::write(Core::Logging::MESSAGE_INFO, "SAsynchronousTaskManager: Initialized with %u workers.", mPoolSize);
            }

            SAsynchronousTaskManager::~SAsynchronousTaskManager(void)
            {
                // Kill any active workers
                for (auto it = mActiveWorkers.begin(); it != mActiveWorkers.end(); it++)
                    delete (*it);

                // Kill inactive workers
                for (auto it = mIdleWorkers.begin(); it != mIdleWorkers.end(); it++)
                    delete (*it);
            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
