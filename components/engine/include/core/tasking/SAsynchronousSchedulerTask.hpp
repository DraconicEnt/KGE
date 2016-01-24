
#ifndef _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
#define _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_

#include <easydelegate/easydelegate.hpp>

#include <support/common.hpp>

#include <support/support.hpp>

#include <support/tasking/ITask.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            class SAsynchronousSchedulerTask : public Support::Tasking::ITask
            {
                // Public Methods
                public:
                    static SAsynchronousSchedulerTask *getPointer(void);
                    static void destroy(void);

                    void initialize(void);

                    bool tick(const Common::F32 &deltaTime);

                    void deinitialize(void);



                // Private Methods
                private:
                    SAsynchronousSchedulerTask(void);
                    ~SAsynchronousSchedulerTask(void);

                // Public Members
                public:
                    Support::Mutex mMutex;

                // Private Members
                private:
                    Support::UnorderedSet<EasyDelegate::IDeferredCaller *> mScheduledTasks;
            };
        } // End NameSpace Tasking
    } // End Namespace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
