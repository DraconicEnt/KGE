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
            class SSynchronousTaskManager : public Support::ISingleton<SSynchronousTaskManager>
            {
                // Public Methods
                public:
                    void tick(const Common::F32 &deltaTime);
                    bool addTask(ITask *task);
                    bool removeTask(ITask *task);

                    SSynchronousTaskManager(void);
                    ~SSynchronousTaskManager(void);

                // Private Members
                private:
                    Support::UnorderedSet<ITask *> mTaskList;
            };
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro

#endif // _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
