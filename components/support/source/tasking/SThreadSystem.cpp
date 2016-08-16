/**
 *  @file SThreadSystem.cpp
 *  @brief Source implementation for the SThreadSystem singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/SSettingsRegistry.hpp>

#include <support/tasking/SThreadSystem.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static SThreadSystem* sInstance = nullptr;

            SThreadSystem* SThreadSystem::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SThreadSystem();

                return sInstance;
            }

            void SThreadSystem::destroy(void)
            {
                delete sInstance;
                sInstance = nullptr;
            }

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

                    // Give the compiler a big hint about the type of object we're using here
                    CThreadSystemTask* task = reinterpret_cast<CThreadSystemTask*>(context->mTask);

                    context->mIsComplete = task->mIsComplete = task->tick(0.00f);
                }
            }

            SThreadSystem::SThreadSystem(void) : mCurrentPhase(254)
            {
                const Common::U8 threadCount = Support::SSettingsRegistry::getPointer()->getValue<Common::U8>("System::RuntimeThreadCount");

                for (Common::U8 iteration = 0; iteration < threadCount; ++iteration)
                {
                    WorkerContext* currentWorker = new WorkerContext();

                    currentWorker->mTask = new CThreadSystemTask();
                    currentWorker->mIsComplete = true;
                    currentWorker->mThread = new Support::Thread(workerThreadLogic, currentWorker);
                    currentWorker->mThread->detach();

                    mInactiveThreads.insert(mInactiveThreads.end(), currentWorker);
                }

                CONSOLE_INFOF("Initialized with %u worker threads.", threadCount);
            }

            SThreadSystem::~SThreadSystem(void)
            {

            }

            void SThreadSystem::update(void)
            {
                // Blow over every thread group we have active for this
                for (Support::Vector<WorkerContext*>& currentGroup: mPhaseProcessing)
                {
                    Support::UnorderedSet<WorkerContext*> completedThreads;

                    // Count how many threads have completed in this group
                    Common::U8 completedThreadCount = 0;
                    for (WorkerContext* context: currentGroup)
                        if (context->mIsComplete)
                            ++completedThreadCount;

                    // If everything in the group has completed, we process all the transactions
                    if (completedThreadCount == currentGroup.size())
                    {
                        for (WorkerContext* context: currentGroup)
                        {
                            mInactiveThreads.push_back(context);

                            auto activeThreadIter = mActiveThreads.find(context);
                            assert(activeThreadIter != mActiveThreads.end());
                            mActiveThreads.erase(activeThreadIter);

                            // Process any transactions
                            CThreadSystemTask* task = reinterpret_cast<CThreadSystemTask*>(context->mTask);

                            assert(task->mDebugMutex.try_lock());
                            while (!task->mTransactions.empty())
                            {
                                Support::Queue<EasyDelegate::IDeferredCaller*> transactionSet = task->mTransactions.front();
                                task->mTransactions.pop();

                                while (!transactionSet.empty())
                                {
                                    EasyDelegate::IDeferredCaller* caller = transactionSet.front();
                                    caller->genericDispatch();
                                    delete caller;

                                    transactionSet.pop();
                                }
                            }

                            task->mDebugMutex.unlock();
                        }

                        // The group is done, so we clear it for this frame
                        currentGroup.clear();
                    }
                }

                // When the active threads is empty, we either completed a phase or this is first update call
                if (mActiveThreads.size() == 0)
                {
                    ++mCurrentPhase = mCurrentPhase >= mThreadPhases.size() ? 0 : mCurrentPhase;

                    // Now we assign the threads for the next phase
                    Support::Vector<Support::Vector<ThreadAction>>& currentPhase = mThreadPhases[mCurrentPhase];

                    // Create the phase mirror (this is used to submit transactions)
                    // FIXME: Perhaps just clear existing phases and allocate any extras needed? mCurrentPhase will never count more.
                    mPhaseProcessing.clear();

                    for (Common::U8 iteration = 0; iteration < currentPhase.size(); iteration++)
                        mPhaseProcessing.insert(mPhaseProcessing.end(), Support::Vector<WorkerContext*>());

                    Common::U8 availableThreadCount = mInactiveThreads.size();

                    // We map all thread actions across the available threads
                    Common::U8 maxThreadIndex = 0;
                    Common::U8 currentThreadIndex = 0;

                    for (Common::U8 threadGroupIndex = 0; threadGroupIndex < currentPhase.size(); ++threadGroupIndex)
                    {
                        Support::Vector<ThreadAction>& threadGroup = currentPhase[threadGroupIndex];

                        // Assign each action
                        for (Common::U8 iteration = 0; iteration < threadGroup.size(); iteration++)
                        {
                            WorkerContext* context = mInactiveThreads[currentThreadIndex];
                            CThreadSystemTask* task = reinterpret_cast<CThreadSystemTask*>(context->mTask);

                            mActiveThreads.insert(mActiveThreads.end(), context);
                            mPhaseProcessing[iteration].insert(mPhaseProcessing[iteration].end(), context);

                            // Push our actions
                            assert(task->mDebugMutex.try_lock());
                            task->mThreadActions.push(std::make_pair(threadGroupIndex, threadGroup[iteration]));
                            task->mDebugMutex.unlock();

                            ++currentThreadIndex %= mInactiveThreads.size();
                            ++maxThreadIndex = maxThreadIndex >= mInactiveThreads.size() ? mInactiveThreads.size() - 1 : maxThreadIndex;
                        }
                    }

                    // Trigger the threads to run only after the assignments are done
                   for (Common::U8 iteration = 0; iteration < maxThreadIndex; ++iteration)
                        mInactiveThreads[iteration]->mIsComplete = false;

                    // Erase the threads we are currently using
                    mInactiveThreads.erase(mInactiveThreads.begin(), mInactiveThreads.begin() + maxThreadIndex);
                }
            }

            void SThreadSystem::addPhase(Support::Vector<Support::Vector<SThreadSystem::ThreadAction>>& newPhase)
            {
                // Return the reference to the actual stored vector, the one above is a temporary stack element
                mThreadPhases.push_back(newPhase);
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
                assert(mDebugMutex.try_lock());

                assert(mThreadActions.size() != 0);

                auto nextActionData = mThreadActions.front();
                mThreadActions.pop();

                Support::Queue<EasyDelegate::IDeferredCaller*> transaction = nextActionData.second->invoke();
                mTransactions.push(transaction);

                mDebugMutex.unlock();

                return mThreadActions.size() == 0;
            }

            void CThreadSystemTask::deinitialize(void)
            {

            }
        } // End NameSpace Tasking
    } // End NameSpace Support
} // End NameSpace Kiaro
