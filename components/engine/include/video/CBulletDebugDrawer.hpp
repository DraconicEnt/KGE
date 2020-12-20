/**
 *  @file CBulletDebugDrawer.hpp
 *  @brief Include file declaring the CBulletDebugDrawer class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_ENGINE_VIDEO_CBULLETDEBUGDRAWER_HPP_
#define _INCLUDE_ENGINE_VIDEO_CBULLETDEBUGDRAWER_HPP_

#include <bullet/LinearMath/btIDebugDraw.h>

#include <support/common.hpp>
#include <support/types.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            /**
             *  @brief The CBulletDebugDrawer is a interface class to Bullet for rendering debug information about
             *  the physical simulation to the scene.
             */
            class CBulletDebugDrawer : public btIDebugDraw
            {
                // Public Methods
                public:
                    CBulletDebugDrawer(void);

                    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);

                    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                                  Kiaro::Common::S32 lifeTime, const btVector3 &color);

                    virtual void reportErrorWarning(const char* warningString);

                    virtual void draw3dText(const btVector3 &location, const char* textString);

                    virtual void setDebugMode(Kiaro::Common::S32 debugMode);

                    virtual int getDebugMode() const;

                // Private Members
                private:
                    Common::S32 mDebugMode;
            };
        } // End NameSpace Video
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_ENGINE_VIDEO_CBULLETDEBUGDRAWER_HPP_
