/**
 */

#include <support/SSettingsRegistry.hpp>
#include <support/tasking/SAsynchronousTaskManager.hpp>

#include <support/tasking/SThreadSystem.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static void workerThreadLogic(WorkerContext* context)
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
                        std::this_thread::sleep_for(std::chrono::milliseconds(32));
                        continue;
                    }

                    context->mIsComplete = context->mTask->tick(0.00f);
                }
            }

            SThreadSystem::SThreadSystem(void) : mCurrentPhase(254)
            {
                const Common::U8 threadCount = Support::SSettingsRegistry::getPointer()->getValue<Common::U8>("System::RuntimeThreadCount");

                for (Common::U8 iteration = 0; iteration < threadCount; ++iteration)
                {
                    WorkerContext* currentWorker = new WorkerContext();

                    currentWorker->mTask = nullptr;
                    currentWorker->mIsComplete = true;
                    currentWorker->mThread = new Support::Thread(workerThreadLogic, currentWorker);
                    currentWorker->mThread->detach();

                    mInactiveThreads.insert(mInactiveThreads.end(), currentWorker);
                }
            }

            void SThreadSystem::update(void)
            {
                Support::UnorderedSet<WorkerContext*> completedWorkers;

                // Look for any thread contexts that have completed
                for (auto it = mActiveThreads.begin(); it != mActiveThreads.end(); it++)
                {
                    WorkerContext* context = *it;

                    if (context->mIsComplete)
                    {
                        completedWorkers.insert(completedWorkers.end(), context);

                        // Process any transactions
                        CThreadSystemTask* task = reinterpret_cast<CThreadSystemTask*>(context->mTask);
                        while (!task->mTransactions.empty())
                        {
                            Support::Queue<EasyDelegate::IDeferredCaller*>& transactionSet = task->mTransactions.front();
                            task->mTransactions.pop();

                            while (!transactionSet.empty())
                            {
                                EasyDelegate::IDeferredCaller* caller = transactionSet.front();
                                transactionSet.pop();

                                caller->genericDispatch();
                            }
                        }
                    }
                }

                // Now we move the contexts over
                for (auto it = completedWorkers.begin(); it != completedWorkers.end(); it++)
                {
                    WorkerContext* context = *it;
                    mInactiveThreads.insert(mInactiveThreads.end(), context);
                }

                // When the active threads is empty, we either completed a phase or this is first update call
                if (mActiveThreads.size() == 0)
                {
                    ++mCurrentPhase = mCurrentPhase >= mThreadPhases.size() ? 0 : mCurrentPhase;

                    // Now we assign the threads for the next phase
                    const Support::Vector<Support::Vector<ThreadAction>>& currentPhase = mThreadPhases[mCurrentPhase];

                    const Common::U8 division = currentPhase.size() / mInactiveThreads.size();
                    const Common::U8 spillOver = currentPhase.size() % mInactiveThreads.size();

                    /* TODO: Phase processing assignment */
                }
            }

            Support::Vector<Support::Vector<SThreadSystem::ThreadAction>>& SThreadSystem::addPhase(void)
            {
                Support::Vector<Support::Vector<ThreadAction>> newPhase;

                // Return the reference to the actual stored vector, the one above is a temporary stack element
                mThreadPhases.insert(mThreadPhases.end(), newPhase);
                return *mThreadPhases.end();
            }

            Support::Vector<Support::Vector<SThreadSystem::ThreadAction>>& SThreadSystem::getPhase(const size_t phase)
            {
                return mThreadPhases[phase];
            }

            void CThreadSystemTask::initialize(void)
            {

            }

            bool CThreadSystemTask::tick(const Common::F32 deltaTimeSeconds)
            {
                assert(!mThreadActions.empty());

                SThreadSystem::ThreadAction nextAction = mThreadActions.front();
                mThreadActions.pop();

                Support::Queue<EasyDelegate::IDeferredCaller*> transaction = nextAction->invoke();
                mTransactions.push(transaction);

                return true;
            }

            void CThreadSystemTask::deinitialize(void)
            {

            }
        } // End NameSpace Tasking
    } // End NameSpace Support
} // End NameSpace Kiaro
