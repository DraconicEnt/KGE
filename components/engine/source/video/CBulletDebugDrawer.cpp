/**
 *  @file CBulletDebugDrawer.cpp
 *  @brief Implementation file for the CBulletDebugDrawer class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <video/CBulletDebugDrawer.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            CBulletDebugDrawer::CBulletDebugDrawer(void)
            {
             //   mIrrlicht = irrlicht;
            }

            void CBulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
            {
              //  irr::video::IVideoDriver* video = mIrrlicht->getVideoDriver();
                Support::Vector3DF startPosition(from.x(), from.y(), from.z());
                Support::Vector3DF endPosition(to.x(), to.y(), to.z());
                Support::ColorRGBA drawColor(255, color.x(), color.y(), color.z());

               // video->draw3DLine(startPosition, endPosition, drawColor);
            }

            void CBulletDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
                                                      btScalar distance, Kiaro::Common::S32 lifeTime,
                                                      const btVector3 &color)
            {
              //  irr::video::IVideoDriver* video = mIrrlicht->getVideoDriver();
                btVector3 bulletEndPosition = PointOnB + (distance * normalOnB);
                Support::Vector3DF startPosition(PointOnB.x(), PointOnB.y(), PointOnB.z());
                Support::Vector3DF endPosition(bulletEndPosition.x(), bulletEndPosition.y(), bulletEndPosition.z());
                Support::ColorRGBA drawColor(255, color.x(), color.y(), color.z());

               // video->draw3DLine(startPosition, endPosition, drawColor);
            }

            void CBulletDebugDrawer::reportErrorWarning(const char* warningString)
            {
                CONSOLE_WARNING(warningString);
            }

            void CBulletDebugDrawer::draw3dText(const btVector3 &location, const char* textString)
            {
               // irr::video::IVideoDriver* video = mIrrlicht->getVideoDriver();
                //video->draw3DText
            }

            void CBulletDebugDrawer::setDebugMode(Common::S32 debugMode)
            {
                mDebugMode = debugMode;
            }

            Common::S32 CBulletDebugDrawer::getDebugMode() const
            {
                return mDebugMode;
            }
        } // End NameSpace Video
    } // End NameSpace Engine
} // End NameSpace Kiaro
