/**
 *  @file CBulletDebugDrawer.hpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_ENGINE_CBULLETDEBUGDRAWER_HPP_
#define _INCLUDE_ENGINE_CBULLETDEBUGDRAWER_HPP_

#include <LinearMath/btIDebugDraw.h>
#include <irrlicht.h>

#include <engine/common.hpp>

namespace Kiaro
{
    namespace Engine
    {
        class CBulletDebugDrawer : public btIDebugDraw
        {
            // Public Methods
            public:
                CBulletDebugDrawer(irr::IrrlichtDevice *irrlicht);

                virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
                virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance, Kiaro::Common::S32 lifeTime,const btVector3& color);
                virtual void reportErrorWarning(const char* warningString);
                virtual void draw3dText(const btVector3& location,const char* textString);
                virtual void setDebugMode(Kiaro::Common::S32 debugMode);
                virtual int getDebugMode() const;

            // Private Members
            private:
                Kiaro::Common::S32 mDebugMode;
                irr::IrrlichtDevice *mIrrlicht;
        };
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_ENGINE_CBULLETDEBUGDRAWER_HPP_
