/**
 *
 */

#include <support/tasking/SSynchronousTaskManager.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static SSynchronousTaskManager* sInstance = nullptr;

            SSynchronousTaskManager* SSynchronousTaskManager::getPointer(void)
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

            void SSynchronousTaskManager::tick(const Common::F32& deltaTime)
            {
                for (auto it = mTaskList.begin(); it != mTaskList.end(); it++)
                    if ((*it)->tick(deltaTime))
                        mTaskList.erase(it);
            }

            bool SSynchronousTaskManager::addTask(ITask* task)
            {
                if (!task)
                    throw std::runtime_error("SSynchronousTaskManager: Cannot add a NULL task.");

                if (mTaskList.count(task) != 0)
                    return false;

                mTaskList.insert(mTaskList.end(), task);
                return true;
            }

            bool SSynchronousTaskManager::removeTask(ITask* task)
            {
                if (!task)
                    throw std::runtime_error("SSynchronousTaskManager: Cannot remove a NULL task.");

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
