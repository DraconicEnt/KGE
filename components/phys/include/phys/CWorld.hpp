/**
 *  @file CWorld.hpp
 */

#ifndef _INCLUDE_PHYS_CWORLD_HPP_
#define _INCLUDE_PHYS_CWORLD_HPP_

#include <btBulletDynamicsCommon.h>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Phys
    {
        class CWorld
        {
            public:
                CWorld(void);
                ~CWorld(void);

                void update(const Common::F32& deltaTimeSeconds);



            private:
                btBroadphaseInterface* mBroadphase;
                btCollisionDispatcher* mCollisionDispatcher;
                btConstraintSolver* mConstraintSolver;
                btDefaultCollisionConfiguration* mCollisionConfiguration;
                btDiscreteDynamicsWorld* mPhysicalWorld;
        };
    } // End NameSpace Phys
} // End NameSpace Kiaro
#endif
