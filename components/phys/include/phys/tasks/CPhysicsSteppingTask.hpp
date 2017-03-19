/**
 *  @file CPhysicsSteppingTask.hpp
 *  @brief Include file declaring the CPhysicsSteppingTask class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <phys/CSimulation.hpp>
#include <support/tasking/ITask.hpp>

namespace Kiaro
{
    namespace Phys
    {
        namespace Tasks
        {
            class CPhysicsSteppingTask : public Support::Tasking::ITask
            {
                // Private Members
                private:
                    //! The simulation object we are clocking.
                    CSimulation* mSimulation;

                // Public Methods
                public:
                    CPhysicsSteppingTask(CSimulation* simulation);

                    //! Initializes the task, allocating any necessary resources.
                    void initialize(void);

                    /**
                     *  @brief Ticks the task. This is primarily useful for when the task is being
                     *  executed by the SSynchrnousTaskManager.
                     *  @param deltaTimeSeconds The amount of time in seconds that has passed since the
                     *  last tick.
                     */
                    bool tick(const Common::F32 deltaTimeSeconds);

                    //! Deinitializes the task, destroying any associated resources.
                    void deinitialize(void);
            };
        }
    }
}

