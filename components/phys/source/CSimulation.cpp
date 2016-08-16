/**
 *  @file CSimulation.cpp
 *  @brief Source file implementing the CSimulation class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <phys/CSimulation.hpp>

namespace Kiaro
{
    namespace Phys
    {
        CSimulation::CSimulation(void)
        {
            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
            mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mConstraintSolver = new btSequentialImpulseConstraintSolver();
            mPhysicalWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mConstraintSolver, mCollisionConfiguration);
        }

        CSimulation::~CSimulation(void)
        {
            delete mPhysicalWorld;
            mPhysicalWorld = nullptr;
            delete mConstraintSolver;
            mConstraintSolver = nullptr;
            delete mBroadphase;
            mBroadphase = nullptr;
            delete mCollisionDispatcher;
            mCollisionDispatcher = nullptr;
            delete mCollisionConfiguration;
            mCollisionConfiguration = nullptr;
        }

        void CSimulation::update(const Common::F32 deltaTimeSeconds)
        {
            mPhysicalWorld->stepSimulation(deltaTimeSeconds);
        }

        void CSimulation::setDebugRenderer(IDebugRenderer* renderer)
        {
            // We shouldn't already have a debug renderer in here
            assert(!mDebugRenderer);
            mDebugRenderer = renderer;
            mPhysicalWorld->setDebugDrawer(renderer);
        }
    } // End NameSpace Phys
} // End NameSpace Kiaro
