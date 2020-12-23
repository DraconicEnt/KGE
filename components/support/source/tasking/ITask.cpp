/**
 *  @file ITask.cpp
 *  @brief Source file implementing the ITask class and methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/tasking/ITask.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            ITask::ITask(void) : mProcessingIndex(0), mThreadWeight(0)
            {

            }

            ITask::~ITask()
            {

            }

            Common::U32 ITask::getProcessingIndex(void)
            {
                return mProcessingIndex;
            }

            Common::U32 ITask::getThreadWeight(void)
            {
                return mThreadWeight;
            }

            const Support::UnorderedSet<Common::U32>& ITask::getResources(void)
            {
                return mResources;
            }
        } // End NameSpace Tasking
    } // End NameSpace Engine
} // End NameSpace Kiaro
