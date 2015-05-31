/**
 *
 */

#include <core/tasking/SSynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            static SSynchronousTaskManager *sInstance = NULL;

            SSynchronousTaskManager *SSynchronousTaskManager::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SSynchronousTaskManager;

                return sInstance;
            }

            void SSynchronousTaskManager::destroy(void)
            {
                if (sInstance)
                {
                    delete sInstance;
                    sInstance = NULL;
                }
            }

            void SSynchronousTaskManager::tick(const Kiaro::Common::F32 &deltaTime)
            {
                for (auto it = mTaskList.begin(); it != mTaskList.end(); it++)
                    if ((*it)->tick(deltaTime))
                        mTaskList.erase(it);
            }

            bool SSynchronousTaskManager::addTask(Kiaro::Core::Tasking::CTask *task)
            {
                if (!task)
                {
                    throw std::runtime_error("SSynchronousTaskManager: Cannot add a NULL task.");
                    return false;
                }

                if (mTaskList.count(task) != 0)
                    return false;

                mTaskList.insert(mTaskList.end(), task);
                return true;
            }

            bool SSynchronousTaskManager::removeTask(Kiaro::Core::Tasking::CTask *task)
            {
                if (!task)
                {
                    throw std::runtime_error("SSynchronousTaskManager: Cannot remove a NULL task.");
                    return false;
                }

                return mTaskList.erase(task) != 0;
            }

            SSynchronousTaskManager::SSynchronousTaskManager(void)
            {
                std::cout << "SSynchronousTaskManager: Initialized. " << std::endl;
            }

            SSynchronousTaskManager::~SSynchronousTaskManager(void)
            {

            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
