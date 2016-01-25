/**
 *  @file SAsynchronousSchedulerTask.hpp
 *  @brief Main include file for the tasking subsystem components.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */
 
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
            /**
             *  @brief A class representing a task that may be executed asynchronously from
             *  the rest of the engine.
             */
            class SAsynchronousSchedulerTask : public Support::Tasking::ITask
            {
                // Public Members
                public:
                    Support::Mutex mMutex;

                // Private Members
                private:
                    Support::UnorderedSet<EasyDelegate::IDeferredCaller*> mScheduledTasks;
                    
                // Public Methods
                public:
                    static SAsynchronousSchedulerTask* getPointer(void);
                    static void destroy(void);

                    void initialize(void);

                    bool tick(const Common::F32& deltaTime);

                    void deinitialize(void);

                // Private Methods
                private:
                    SAsynchronousSchedulerTask(void);
                    ~SAsynchronousSchedulerTask(void);
            };
        } // End NameSpace Tasking
    } // End Namespace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_TASKER_SASYNCHRONOUSSCHEDULERTASK_HPP_
