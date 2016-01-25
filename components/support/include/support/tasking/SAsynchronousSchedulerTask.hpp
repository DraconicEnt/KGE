/**
 *  @file SSynchronousSchedulerTask.hpp
 *  @brief Include file declaring the Support::Tasking::SAsynchronousScheduler singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
#define _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_

#include <easydelegate/easydelegate.hpp>

#include <support/common.hpp>
#include <support/support.hpp>
#include <support/tasking/ITask.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            class SAsynchronousSchedulerTask : public ITask
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
                    Support::UnorderedSet<EasyDelegate::IDeferredCaller*> mScheduledTasks;
            };
        } // End NameSpace Tasking
    } // End Namespace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
