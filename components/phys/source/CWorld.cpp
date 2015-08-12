/**
 */

#include <phys/CWorld.hpp>

namespace Kiaro
{
    namespace Phys
    {
        CWorld::CWorld(void)
        {
            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
//            mPhysicalDebugger = new Video::CBulletDebugDrawer(irrlicht);
            mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mConstraintSolver = new btSequentialImpulseConstraintSolver();

            mPhysicalWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mConstraintSolver, mCollisionConfiguration);
            //mPhysicalWorld->setDebugDrawer(mPhysicalDebugger);
        }

        CWorld::~CWorld(void)
        {
            delete mPhysicalWorld;
            mPhysicalWorld = NULL;

            delete mConstraintSolver;
            mConstraintSolver = NULL;

            delete mBroadphase;
            mBroadphase = NULL;

            delete mCollisionDispatcher;
            mCollisionDispatcher = NULL;

            delete mCollisionConfiguration;
            mCollisionConfiguration = NULL;

          //  delete mPhysicalDebugger;
           // mPhysicalDebugger = NULL;
        }

        void CWorld::update(const Common::F32& deltaTimeSeconds)
        {

        }
    } // End NameSpace Phys
} // End NameSpace Kiaro
