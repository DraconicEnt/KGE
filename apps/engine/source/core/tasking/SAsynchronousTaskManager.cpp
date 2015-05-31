/**
 */

#include <core/tasking/SAsynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            static SAsynchronousTaskManager *sInstance = NULL;

            static void workerThreadLogic(Kiaro::Core::Tasking::WorkerContext *context)
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

            SAsynchronousTaskManager *SAsynchronousTaskManager::getPointer(const size_t &poolSize)
            {
                if (!sInstance)
                    sInstance = new SAsynchronousTaskManager(poolSize);

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
                    Kiaro::Core::Tasking::WorkerContext *currentWorker = *it;

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

            const size_t &SAsynchronousTaskManager::getWorkerPoolSize(void)
            {
                return mPoolSize;
            }

            bool SAsynchronousTaskManager::addTask(Kiaro::Core::Tasking::CTask *task)
            {
                if (!task)
                {
                    throw std::runtime_error("SAsynchronousTaskManager: Cannot add a NULL task.");
                    return false;
                }

                mScheduledTasks.push(task);
            }

            bool SAsynchronousTaskManager::removeTask(Kiaro::Core::Tasking::CTask *task)
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

            SAsynchronousTaskManager::SAsynchronousTaskManager(const size_t &poolSize) : mPoolSize(poolSize)
            {
                for (size_t iteration = 0; iteration < poolSize; iteration++)
                {
                    Kiaro::Core::Tasking::WorkerContext *currentWorker = new Kiaro::Core::Tasking::WorkerContext;
                    currentWorker->mTask = NULL;
                    currentWorker->mIsComplete = true;
                    currentWorker->mThread = new Kiaro::Support::Thread(workerThreadLogic, currentWorker);

                    mIdleWorkers.insert(mIdleWorkers.end(), currentWorker);
                }

                std::cout << "SAsynchronousTaskManager: Initialized with " << poolSize << " workers." << std::endl;
            }

            SAsynchronousTaskManager::~SAsynchronousTaskManager(void)
            {

            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
