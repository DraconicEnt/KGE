/**
 *  @file CSimulation.hpp
 */

#ifndef _INCLUDE_PHYS_CWORLD_HPP_
#define _INCLUDE_PHYS_CWORLD_HPP_

#include <btBulletDynamicsCommon.h>

#include <support/common.hpp>

#include <phys/IDebugRenderer.hpp>

namespace Kiaro
{
    namespace Phys
    {
        /**
         *  @brief CSimulation is a class that represents a physical simulation in an abstract manner,
         *  allowing us to worry more about the simulation we're trying to represent instead of the
         *  underlaying physics API itself.
         */
        class CSimulation
        {
            // Private Members
            private:
                //! Pointer to the bullet broadphase interface.
                btBroadphaseInterface* mBroadphase;

                //! Pointer to the bullet collision dispatcher.
                btCollisionDispatcher* mCollisionDispatcher;

                //! Pointer to the bullet constraint solver.
                btConstraintSolver* mConstraintSolver;

                //! Pointer to the bullet collision configuration.
                btDefaultCollisionConfiguration* mCollisionConfiguration;

                //! Pointer to the bullet physical world.
                btDiscreteDynamicsWorld* mPhysicalWorld;

                //! Pointer to the current physics debug renderer.
                IDebugRenderer* mDebugRenderer;

            // Public Methods
            public:
                /**
                 *  @brief Parameter-less constructor.
                 */
                CSimulation(void);

                /**
                 *  @brief Standard destructor.
                 */
                ~CSimulation(void);

                /**
                 *  @brief Advances the physical simulation along by deltaTimeSeconds seconds.
                 *  @param deltaTimeSeconds The number of seconds to advance the simulation by.
                 */
                void update(const Common::F32 deltaTimeSeconds);

                /**
                 *  @brief Set the debug renderer of the simulation which will be utilized immediately.
                 *  @param renderer The pointer to the renderer to use. If NULL, then there will be no
                 *  debug rendering.
                 */
                void setDebugRenderer(IDebugRenderer* renderer);
        };
    } // End NameSpace Phys
} // End NameSpace Kiaro
#endif
