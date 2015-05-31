
#ifndef _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
#define _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_

#include <easydelegate.hpp>

#include <core/common.hpp>

#include <support/support.hpp>

#include <core/tasking/CTask.hpp>

namespace Kiaro
{
    namespace Core
    {
        namespace Tasking
        {
            class SAsynchronousSchedulerTask : public CTask
            {
                // Public Methods
                public:
                    static SAsynchronousSchedulerTask *getPointer(void);
                    static void destroy(void);

                    void initialize(void);

                    bool tick(const Kiaro::Common::F32 &deltaTime);

                    void deinitialize(void);



                // Private Methods
                private:
                    SAsynchronousSchedulerTask(void);
                    ~SAsynchronousSchedulerTask(void);

                // Public Members
                public:
                    Kiaro::Support::Mutex mMutex;

                // Private Members
                private:
                    Kiaro::Support::UnorderedSet<EasyDelegate::GenericCachedDelegate *> mScheduledTasks;
            };
        } // End NameSpace Tasking
    } // End Namespace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
