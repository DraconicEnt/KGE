/**
 *  @file SSynchronousTaskManager.hpp
 *  @brief Include file declaring the Support::Tasking::SSynchrnousTaskManager singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
#define _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_

#include <support/common.hpp>

#include <support/tasking/ITask.hpp>
#include <support/support.hpp>
#include <support/ISingleton.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            /**
             *  @brief A task processor implementation that dispatches tasks within the context of the
             *  main thread.
             */
            class SSynchronousTaskManager : public Support::ISingleton<SSynchronousTaskManager>
            {
                // Private Members
                private:
                    //! A set of tasks to process.
                    Support::UnorderedSet<ITask *> mTaskList;

                // Public Methods
                public:
                    /**
                     *  @brief Ticks all tasks along, taking into consideration time passage where
                     *  necessary.
                     *  @param deltaTime The amount of time in seconds that has passed.
                     */
                    void tick(const Common::F32 deltaTime);

                    /**
                     *  @brief Adds a new task to the task manager.
                     *  @param task The task to add.
                     *  @return A boolean representing whether or not the task was added successfully.
                     *  @throw std::runtime_error Thrown when the input task is null.
                     */
                    bool addTask(ITask *task);

                    /**
                     *  @brief Removes a task from the task manager.
                     *  @param task The task to remove.
                     *  @return A boolean representing whether or not the task was added successfully.
                     *  @throw std::runtime_error Thrown when the input task is null.
                     */
                    bool removeTask(ITask *task);

                    //! Parameter-less constructor.
                    SSynchronousTaskManager(void);

                    //! Standard destructor.
                    ~SSynchronousTaskManager(void);
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro

#endif // _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
