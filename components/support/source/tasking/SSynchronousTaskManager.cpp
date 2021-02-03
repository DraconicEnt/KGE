/**
 *  @file SSynchronousTaskManager.cpp
 *  @brief Source file implementing the SSynchronousTaskManager singleton class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <support/tasking/SSynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            void SSynchronousTaskManager::tick(const Common::F32 deltaTime)
            {
                for (auto it = mTaskList.begin(); it != mTaskList.end(); it++)
                {
                    if ((*it)->tick(deltaTime))
                    {
                        mTaskList.erase(it);
                    }
                }
            }

            bool SSynchronousTaskManager::addTask(ITask* task)
            {
                if (!task)
                {
                    throw std::runtime_error("SSynchronousTaskManager: Cannot add a NULL task.");
                }

                if (mTaskList.count(task) != 0)
                {
                    return false;
                }

                mTaskList.insert(mTaskList.end(), task);
                return true;
            }

            bool SSynchronousTaskManager::removeTask(ITask* task)
            {
                if (!task)
                {
                    throw std::runtime_error("SSynchronousTaskManager: Cannot remove a NULL task.");
                }

                return mTaskList.erase(task) != 0;
            }

            SSynchronousTaskManager::SSynchronousTaskManager(void)
            {
                CONSOLE_INFO("SSynchronousTaskManager: Initialized.");
            }

            SSynchronousTaskManager::~SSynchronousTaskManager(void)
            {
            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
