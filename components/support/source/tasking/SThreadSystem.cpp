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

#include <support/SProfiler.hpp>
#include <support/SSettingsRegistry.hpp>

#include <support/tasking/SThreadSystem.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static void workerThreadLogic(SThreadSystem::CThreadContext* threadContext)
            {
                // TODO (Robert MacGregor #9): Detect thread error?
                if (!threadContext)
                    return;

                // Keep running, wait for tasks
                while (!threadContext->mShouldTerminate)
                {
                    if (threadContext->mIsComplete)
                    {
                        // Nothing to do, just sleep for a moment before restarting the loop.
                        std::this_thread::sleep_for(std::chrono::milliseconds(32));
                        continue;
                    }

                    SThreadSystem::IThreadedTask* currentTask = threadContext->mThreadTasks.front().second;
                    currentTask->mIsComplete = currentTask->tick(0.00f);

                    if (currentTask->mIsComplete)
                    {
                        assert(threadContext->mDebugMutex.try_lock());

                        threadContext->mTransactions.push(currentTask->getTransaction());
                        threadContext->mThreadTasks.pop();

                        threadContext->mDebugMutex.unlock();
                    }

                    threadContext->mIsComplete = threadContext->mThreadTasks.size() == 0;
                }
            }

            SThreadSystem::SThreadSystem(void) : mCurrentPhase(254)
            {
                const Common::U8 threadCount = Support::SSettingsRegistry::getInstance()->getValue<Common::U8>("System::RuntimeThreadCount");

                for (Common::U8 iteration = 0; iteration < threadCount; ++iteration)
                {
                    CThreadContext* currentWorker = new CThreadContext();
                    mInactiveThreads.insert(mInactiveThreads.end(), currentWorker);
                }

                CONSOLE_INFOF("Initialized with %u worker threads.", threadCount);
            }

            SThreadSystem::~SThreadSystem(void)
            {

            }

            bool SThreadSystem::update(void)
            {
                PROFILER_BEGIN(ThreadSystem);

                // Blow over every thread group we have active for this
                Support::UnorderedSet<CThreadContext*> doneContexts;

                for (Support::Vector<CThreadContext*>& currentGroup: mPhaseProcessing)
                {
                    // Count how many threads have completed in this group
                    Common::U8 completedThreadCount = 0;
                    for (CThreadContext* context: currentGroup)
                        if (context->mIsComplete)
                            ++completedThreadCount;

                    // If everything in the group has completed, we process all the transactions
                    if (completedThreadCount == currentGroup.size())
                    {
                        for (CThreadContext* context: currentGroup)
                        {
                            doneContexts.insert(doneContexts.end(), context);

                            assert(context->mDebugMutex.try_lock());
                            while (!context->mTransactions.empty())
                            {
                                Support::Queue<EasyDelegate::IDeferredCaller*>& transactionSet = context->mTransactions.front();
                                context->mTransactions.pop();

                                while (!transactionSet.empty())
                                {
                                    EasyDelegate::IDeferredCaller* caller = transactionSet.front();
                                    caller->genericDispatch();
                                    delete caller;

                                    transactionSet.pop();
                                }

                                assert(transactionSet.size() == 0);
                            }
                            context->mDebugMutex.unlock();
                        }

                        // The group is done, so we clear it for this frame
                        currentGroup.clear();
                    }
                    else // If the group is still running, we blow out any contexts still in it from the done contexts
                        for (CThreadContext* context: currentGroup)
                        {
                            auto contextIter = doneContexts.find(context);
                            if (contextIter != doneContexts.end())
                                doneContexts.erase(contextIter);
                        }
                }

                // Now we blow through done contexts and do transfers
                for (CThreadContext* context: doneContexts)
                {
                    auto contextIter = mActiveThreads.find(context);

                    assert(contextIter != mActiveThreads.end());
                    mActiveThreads.erase(contextIter);
                    mInactiveThreads.push_back(context);
                }

                // When the active threads is empty, we either completed a phase or this is first update call
                bool completedFrame = false;

                if (mActiveThreads.size() == 0)
                {
                    // Generate phases if we need to.
                    if (mPendingAddTasks.size() != 0 || mPendingRemoveTasks.size() != 0)
                    {
                        for (IThreadedTask* task: mPendingRemoveTasks)
                            mTasks.erase(task);
                        for (IThreadedTask* task: mPendingAddTasks)
                            mTasks.insert(task);

                        mPendingAddTasks.clear();
                        mPendingRemoveTasks.clear();
                        this->generatePhases();
                    }

                    // Don't continue if we don't have any actual phases.
                    if (mThreadPhases.size() == 0)
                    {
                        PROFILER_END(ThreadSystem);
                        return false;
                    }

                    // If the current phase is the last phase, then we're done processing this frame
                    completedFrame = mCurrentPhase == mThreadPhases.size() - 1;
                    ++mCurrentPhase = mCurrentPhase >= mThreadPhases.size() ? 0 : mCurrentPhase;

                    // Now we assign the threads for the next phase
                    Support::Vector<Support::Vector<IThreadedTask*>>& currentPhase = mThreadPhases[mCurrentPhase];

                    // Create the phase mirror (this is used to submit transactions)
                    // FIXME: Perhaps just clear existing phases and allocate any extras needed? mCurrentPhase will never count more.
                    mPhaseProcessing.clear();
                    for (Common::U8 iteration = 0; iteration < currentPhase.size(); iteration++)
                        mPhaseProcessing.insert(mPhaseProcessing.end(), Support::Vector<CThreadContext*>());

                    Common::U8 availableThreadCount = mInactiveThreads.size();

                    // We map all thread actions across the available threads
                    Common::U8 maxThreadIndex = 0;
                    Common::U8 currentThreadIndex = 0;

                    for (Common::U8 threadGroupIndex = 0; threadGroupIndex < currentPhase.size(); ++threadGroupIndex)
                    {
                        Support::Vector<IThreadedTask*>& threadGroup = currentPhase[threadGroupIndex];

                        // Assign each action
                        for (Common::U8 iteration = 0; iteration < threadGroup.size(); iteration++)
                        {
                            CThreadContext* context = mInactiveThreads[currentThreadIndex];

                            mActiveThreads.insert(mActiveThreads.end(), context);
                            mPhaseProcessing[threadGroupIndex].insert(mPhaseProcessing[threadGroupIndex].end(), context);

                            // Push our actions
                            assert(context->mDebugMutex.try_lock());
                            context->mThreadTasks.push(std::make_pair(threadGroupIndex, threadGroup[iteration]));
                            context->mDebugMutex.unlock();

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

                PROFILER_END(ThreadSystem);
                return completedFrame;
            }

            bool SThreadSystem::addTask(IThreadedTask* task)
            {
                if (mTasks.find(task) != mTasks.end() || mPendingAddTasks.find(task) != mPendingAddTasks.end())
                    return false;
                mPendingAddTasks.insert(task);
                return true;
            }

            bool SThreadSystem::removeTask(IThreadedTask* task)
            {
                if (mTasks.find(task) == mTasks.end() || mPendingRemoveTasks.find(task) != mPendingRemoveTasks.end())
                    return false;
                mPendingRemoveTasks.insert(task);
                return true;
            }

            void SThreadSystem::generatePhases(void)
            {
                mThreadPhases.clear();

                CONSOLE_DEBUGF("Generating for %u total input tasks.", mTasks.size());

                // First, we group into threading indexes
                Support::Set<Common::U32> processingIndexes;
                Support::UnorderedMap<Common::U32, Support::UnorderedSet<IThreadedTask*>> indexGroups;
                for (IThreadedTask* task: mTasks)
                {
                    const Common::U32 processingIndex = task->getProcessingIndex();
                    if (indexGroups.find(processingIndex) == indexGroups.end())
                    {
                        processingIndexes.insert(processingIndex);
                        indexGroups[processingIndex] = Support::UnorderedSet<IThreadedTask*>();
                    }
                    indexGroups[processingIndex].insert(task);
                }

                // Process for each group and generate phase information
                for (Common::U32 index: processingIndexes)
                {
                    Support::Vector<Support::Vector<IThreadedTask*>> generatedPhase;

                    // For this phase index, we need to generate groups that contain tasks that share resources. This is used
                    // to determine when data can finally be written to memory safely for this frame via the transaction API
                    Support::UnorderedMap<Common::U32, Support::UnorderedSet<IThreadedTask*>> sharedResources;
                    for (IThreadedTask* currentTask: indexGroups[index])
                        for (const Common::U32 currentResource: currentTask->getResources())
                            for (IThreadedTask* testedTask: indexGroups[index])
                            {
                                if (testedTask == currentTask)
                                        continue;

                                const Support::UnorderedSet<Common::U32>& testedResources = testedTask->getResources();
                                for (const Common::U32 testedResource: testedResources)
                                    if (testedResource == currentResource)
                                    {
                                        if (sharedResources.find(testedResource) == sharedResources.end())
                                            sharedResources[testedResource] = Support::UnorderedSet<IThreadedTask*>();

                                        sharedResources[testedResource].insert(testedTask);
                                        sharedResources[testedResource].insert(currentTask);
                                    }
                            }

                    // Once we know where the resource conflicts are, we use this to generate groups that are as small as possible.
                    Support::UnorderedSet<IThreadedTask*> conflictedTasks;
                    for (auto resourceIDTasks: sharedResources)
                        for (IThreadedTask* task: resourceIDTasks.second)
                            conflictedTasks.insert(task);

                    CONSOLE_DEBUGF("Found %u resource conflicts in phase %u.", conflictedTasks.size(), index);
                    Support::UnorderedSet<IThreadedTask*> remainingTasks = Support::UnorderedSet<IThreadedTask*>(indexGroups[index]);

                    // Generate groups on resource conflicts
                    Support::Vector<IThreadedTask*> conflictedGroup;
                    for (auto resourceIDTasks: sharedResources)
                        for (IThreadedTask* task: resourceIDTasks.second)
                            if (remainingTasks.find(task) != remainingTasks.end())
                            {
                                remainingTasks.erase(task);
                                conflictedGroup.push_back(task);
                            }

                    if (conflictedGroup.size() != 0)
                        generatedPhase.push_back(conflictedGroup);

                    // FIXME: We should unify types so we can reduce some code here
                    // FIXME: We always put everything into the same group at the moment. We need to perform grouping such that tasks
                    // in the same group are safe to commit when all tasks complete. Thus, the smaller the grouping, the better.
                    const Common::U32 expectedSize = indexGroups[index].size() - conflictedTasks.size();

                    CONSOLE_ASSERTF(remainingTasks.size() == expectedSize, "Expected %u tasks to be remaining after grouping, but actually had %u!", expectedSize, remainingTasks.size());
                    for (IThreadedTask* task: remainingTasks)
                    {
                        Support::Vector<IThreadedTask*> generatedGroup;
                        generatedGroup.push_back(task);
                        generatedPhase.push_back(generatedGroup);
                    }

                    mThreadPhases.push_back(generatedPhase);
                }

                CONSOLE_DEBUGF("Generated %u phases.", mThreadPhases.size());
                Common::U32 phaseIndex = 0;
                for (auto phaseData: mThreadPhases)
                {
                    CONSOLE_DEBUGF("Phase %u has %u groups.", phaseIndex, phaseData.size());

                    Common::U32 groupIndex = 0;
                    for (auto groupData: phaseData)
                    {
                        CONSOLE_DEBUGF("Phase %u, group %u has %u tasks.", phaseIndex, groupIndex, groupData.size());
                        ++groupIndex;
                    }
                    ++phaseIndex;
                }
            }

            void SThreadSystem::addPhase(Support::Vector<Support::Vector<IThreadedTask*>>& newPhase)
            {
                // Return the reference to the actual stored vector, the one above is a temporary stack element
                mThreadPhases.push_back(newPhase);
            }

            Support::Vector<Support::Vector<SThreadSystem::IThreadedTask*>>& SThreadSystem::getPhase(const size_t phase)
            {
                return mThreadPhases[phase];
            }

            SThreadSystem::CThreadContext::CThreadContext(void) : mIsComplete(true), mShouldTerminate(false)
            {
                mThread = new Support::Thread(workerThreadLogic, this);
                mThread->detach();
            }

            SThreadSystem::CThreadContext::~CThreadContext(void)
            {
                mShouldTerminate = true;

                // FIXME: This can cause deadlocks if the thread is in an infinite loop somewhere
                mThread->join();
            }

            const Support::Queue<EasyDelegate::IDeferredCaller*> SThreadSystem::IThreadedTask::getTransaction(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> result = mTransaction;
                this->clearTransaction();
                return result;
            }

            void SThreadSystem::IThreadedTask::clearTransaction(void)
            {
                mTransaction = Support::Queue<EasyDelegate::IDeferredCaller*>();
            }
        } // End NameSpace Tasking
    } // End NameSpace Support
} // End NameSpace Kiaro
