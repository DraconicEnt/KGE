/**
 */

#ifndef _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_
#define _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_

#include <support/types.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>

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
             *  @detail The threaded runtime is arranged into a series of phases that have thread groups that execute concurrent
             *  to one another in a read only game state environment. Instead of writing changes directly to the game state,
             *  we use EasyDelegate to defer calls into a queue and dispatch that as a single transaction once the main thread is
             *  ready to.
             */
            class SThreadSystem
            {
                // Public Members
                public:
                    typedef EasyDelegate::DelegateSet<Support::Queue<EasyDelegate::IDeferredCaller*>>::StoredDelegateType* ThreadAction;

                // Private Members
                private:
                    //! The current computation phase we are currently on.
                    Common::U8 mCurrentPhase;

                    //! A set of threads currently executing some code.
                    Support::UnorderedSet<WorkerContext*> mActiveThreads;
                    //! A set of idle threads waiting for something to do.
                    Support::UnorderedSet<WorkerContext*> mInactiveThreads;

                    //! The thread phases we are operating with.
                    Support::Vector<Support::Vector<Support::Vector<ThreadAction>>> mThreadPhases;

                public:
                    //! A parameter-less constructor.
                    SThreadSystem(void);

                    //! Update method called from the main thread.
                    void update(void);

                    /**
                     *  @brief Adds a new thread phase to this thread system, returning a reference to it.
                     *  @return A reference to the thread phrase.
                     */
                    Support::Vector<Support::Vector<ThreadAction>>& addPhase(void);

                    /**
                     *  @brief Grabs the existing thread phase by phase ID.
                     *  @param phase The phase ID
                     *  @return A reference to the thread phase.
                     */
                    Support::Vector<Support::Vector<ThreadAction>>& getPhase(const size_t phase);

            };

            /**
             *  @brief A task context for the thread system to make worker threads execute.
             */
            class CThreadSystemTask : public ITask
            {
                // Public Methods
                public:
                    //! Initializes the task, allocating any necessary resources.
                    virtual void initialize(void);

                    /**
                     *  @brief Ticks the task. This is primarily useful for when the task is being
                     *  executed by the SSynchrnousTaskManager.
                     *  @param deltaTimeSeconds The amount of time in seconds that has passed since the
                     *  last tick.
                     */
                    virtual bool tick(const Common::F32 deltaTimeSeconds);

                    //! Deinitializes the task, destroying any associated resources.
                    virtual void deinitialize(void);

                // Public Members
                public:
                    //! A queue of actions for this thread to execute.
                    Support::Queue<SThreadSystem::ThreadAction> mThreadActions;

                    //! A queue of generic deferred callers to dispatch for the transaction.
                    Support::Queue<Support::Queue<EasyDelegate::IDeferredCaller*>> mTransactions;
            };
        } // End NameSpace Tasking
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_TASKING_STHREADSYSTEM_HPP_
