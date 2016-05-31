/**
 *  @file ITask.hpp
 *  @brief Include file declaring the Support::Tasking::ITask interface type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_TASKING_ITASK_HPP_
#define _INCLUDE_SUPPORT_TASKING_ITASK_HPP_

#include <support/support.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            /**
             *  @brief The ITask type is an interface in which individual tasks in the engine
             *  derive their own types from. These tasks may then be executed in the SSynchronousTaskManager
             *  for single-thread execution or SASynchrnousTaskManager for multi-threaded execution.
             */
            class ITask
            {
                    // Public Members
                public:
                    //! A boolean representing whether or not this task instance is currently complete.
                    bool mIsComplete;

                    // Public Methods
                public:
                    //! Initializes the task, allocating any necessary resources.
                    virtual void initialize(void) = 0;

                    /**
                     *  @brief Ticks the task. This is primarily useful for when the task is being
                     *  executed by the SSynchrnousTaskManager.
                     *  @param deltaTimeSeconds The amount of time in seconds that has passed since the
                     *  last tick.
                     */
                    virtual bool tick(const Common::F32 deltaTimeSeconds) = 0;

                    //! Deinitializes the task, destroying any associated resources.
                    virtual void deinitialize(void) = 0;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro

#endif // _INCLUDE_SUPPORT_TASKING_ITASK_HPP_
