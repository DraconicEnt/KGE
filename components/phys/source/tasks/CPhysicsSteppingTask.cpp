/**
 *  @file CPhysicsSteppingTask.cpp
 *  @brief Source file implementing the CPhysicsSteppingTask class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <phys/tasks/CPhysicsSteppingTask.hpp>

namespace Kiaro
{
    namespace Phys
    {
        namespace Tasks
        {
            CPhysicsSteppingTask::CPhysicsSteppingTask(CSimulation* simulation) : mSimulation(simulation)
            {

            }

            void CPhysicsSteppingTask::initialize(void)
            {

            }

            bool CPhysicsSteppingTask::tick(const Common::F32 deltaTimeSeconds)
            {
                mSimulation->update(deltaTimeSeconds);
                return true;
            }

            void CPhysicsSteppingTask::deinitialize(void)
            {

            }
        }
    } // End NameSpace Phys
} // End NameSpace Kiaro
