/**
 *  @file SThreadSystem.hpp
 *  @brief Include file declaring the SThreadSystem singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_
#define _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_

#include <support/ISingleton.hpp>
#include <support/types.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>

#include <support/tasking/SAsynchronousTaskManager.hpp>

#include <easydelegate/easydelegate.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            /**
             *  @brief The SThreadSystem is a singleton class that arranges the engine runtime into a series of threads in an attempt
             *  to make the engine more properly utilize many core systems while still supporting machines with less processor cores
             *  available at hand.
             *  @details The threaded runtime is arranged into a series of phases that have thread groups that execute concurrent
             *  to one another in a read only game state environment. Instead of writing changes directly to the game state,
             *  we use EasyDelegate to defer calls into a queue and dispatch that as a single transaction once the main thread is
             *  ready to and a thread group within the current phase has completed.
             */
            class SThreadSystem : public ISingleton<SThreadSystem>
            {
                // Public Members
                public:
                    class IThreadedTask : public ITask
                    {
                        //! Protected Members
                        protected:
                            //! The queue of transactions to process.
                            Support::Queue<EasyDelegate::IDeferredCaller*> mTransaction;

                        //! Public methods.
                        public:
                            const Support::Queue<EasyDelegate::IDeferredCaller*> getTransaction(void);
                            void clearTransaction(void);
                    };

                    /**
                     *  @brief A task context for the thread system to make worker threads execute using.
                     *  @details The CThreadSystem instance essentially just cycles through any thread actions left to complete for
                     *  this thread and returns false when there is finally no actions left to execute, causing the executing thread
                     *  to go into a idle sleeping state.
                     */
                    class CThreadContext
                    {
                        // Private Members
                        private:
                            //! The internal thread constructed for this context.
                            Support::Thread* mThread;

                        // Public Methods
                        public:
                            CThreadContext(void);
                            ~CThreadContext(void);

                            //! Mutex used for detecting race conditions
                            Support::Mutex mDebugMutex;

                            //! Whether or not the actions queued have completed executed.
                            Support::Atomic<bool> mIsComplete;

                            //! Whether or not the thread should terminate.
                            Support::Atomic<bool> mShouldTerminate;

                        // Public Members
                        public:
                            //! A queue of actions for this thread to execute.
                            Support::Queue<std::pair<Common::U8, IThreadedTask*>> mThreadTasks;

                            //! A queue of generic deferred callers to dispatch for the transaction.
                            Support::Queue<Support::Queue<EasyDelegate::IDeferredCaller*>> mTransactions;
                    };

                // Private Members
                private:
                    //! The current computation phase we are currently on.
                    Common::U8 mCurrentPhase;

                    //! A set of threads currently executing some code.
                    Support::UnorderedSet<CThreadContext*> mActiveThreads;

                    //! A set of idle threads waiting for something to do.
                    Support::Vector<CThreadContext*> mInactiveThreads;

                    //! The thread phases we are operating with.
                    Support::Vector<Support::Vector<Support::Vector<IThreadedTask*>>> mThreadPhases;

                    //! A mirror of the current phase thread groups.
                    Support::Vector<Support::Vector<CThreadContext*>> mPhaseProcessing;

                    //! All tasks to process.
                    Support::UnorderedSet<IThreadedTask*> mTasks;

                    //! All tasks waiting to be added on the next frame.
                    Support::UnorderedSet<IThreadedTask*> mPendingAddTasks;

                    //! All tasks waiting to be removed on the next frame.
                    Support::UnorderedSet<IThreadedTask*> mPendingRemoveTasks;

                // Private Methods
                private:
                    //! Internal method used to generate the thread phase data.
                    void generatePhases(void);

                // Public Methods
                public:
                    /**
                     *  @brief Used to update the threading logic. If all actions for the current phase are completed, this sets up
                     *  execution for the next phase as well as dispatching pending transactions. If there are tasks still running,
                     *  this call does nothing.
                     *  @return True for if a frame (all phases have completed) has completed on this call. False otherwise.
                     */
                    bool update(void);

                    /**
                     *  @brief Adds a new thread phase to this thread system, returning a reference to it.
                     *  @return A reference to the thread phrase.
                     */
                    void addPhase(Support::Vector<Support::Vector<IThreadedTask*>>& newPhase);

                    /**
                     *  @brief Grabs the existing thread phase by phase ID.
                     *  @param phase The phase ID
                     *  @return A reference to the thread phase.
                     */
                    Support::Vector<Support::Vector<IThreadedTask*>>& getPhase(const size_t phase);

                    bool addTask(IThreadedTask* task);
                    bool removeTask(IThreadedTask* task);

                // Private Methods
                public:
                    //! A parameter-less constructor.
                    SThreadSystem(void);

                    //! Standard destructor.
                    ~SThreadSystem(void);
            };
        } // End NameSpace Tasking
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_
