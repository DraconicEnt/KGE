/**
 *  @file SSynchronousTaskManager.hpp
 *  @brief Include file declaring the Support::Tasking::SSynchrnousTaskManager singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_
#define _INCLUDE_SSYNCHRONOUS_TASK_MANAGER_HPP_

#include <support/common.hpp>

#include <core/tasking/ITask.hpp>
#include <support/support.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            class SSynchronousTaskManager
            {
                // Public Methods
                public:
                    static SSynchronousTaskManager *getPointer(void);
                    static void destroy(void);

                    void tick(const Common::F32 &deltaTime);
                    bool addTask(ITask *task);
                    bool removeTask(ITask *task);

                // Private Methods
                private:
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
