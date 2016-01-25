/**
 */

#include <support/tasking/SAsynchronousSchedulerTask.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static SAsynchronousSchedulerTask* sInstance = nullptr;

            SAsynchronousSchedulerTask* SAsynchronousSchedulerTask::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SAsynchronousSchedulerTask;

                return sInstance;
            }

            void SAsynchronousSchedulerTask::destroy(void)
            {
                if (sInstance)
                {
                    delete sInstance;
                    sInstance = NULL;
                }
            }

            void SAsynchronousSchedulerTask::initialize(void)
            {

            }

            bool SAsynchronousSchedulerTask::tick(const Common::F32& deltaTime)
            {
                mMutex.lock();



                mMutex.unlock();

                return false;
            }

            void SAsynchronousSchedulerTask::deinitialize(void)
            {

            }

            SAsynchronousSchedulerTask::SAsynchronousSchedulerTask(void)
            {

            }

            SAsynchronousSchedulerTask::~SAsynchronousSchedulerTask(void)
            {

            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
