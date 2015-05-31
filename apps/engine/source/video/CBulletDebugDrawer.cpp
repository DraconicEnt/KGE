/**
 *  @file CBulletDebugDrawer.cpp
 *  @brief Source code associated with the Kiaro::EngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <video/CBulletDebugDrawer.hpp>

namespace Kiaro
{
    namespace Video
    {
        CBulletDebugDrawer::CBulletDebugDrawer(irr::IrrlichtDevice *irrlicht)
        {
            mIrrlicht = irrlicht;
        }

        void CBulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
        {
            irr::video::IVideoDriver *video = mIrrlicht->getVideoDriver();

            irr::core::vector3df startPosition(from.x(), from.y(), from.z());
            irr::core::vector3df endPosition(to.x(), to.y(), to.z());
            irr::video::SColor drawColor(255, color.x(), color.y(), color.z());

            video->draw3DLine(startPosition, endPosition, drawColor);
        }

        void CBulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, Kiaro::Common::S32 lifeTime, const btVector3& color)
        {
            irr::video::IVideoDriver *video = mIrrlicht->getVideoDriver();

            btVector3 bulletEndPosition = PointOnB + (distance * normalOnB);

            irr::core::vector3df startPosition(PointOnB.x(), PointOnB.y(), PointOnB.z());
            irr::core::vector3df endPosition(bulletEndPosition.x(), bulletEndPosition.y(), bulletEndPosition.z());
            irr::video::SColor drawColor(255, color.x(), color.y(), color.z());

            video->draw3DLine(startPosition, endPosition, drawColor);
        }

        void CBulletDebugDrawer::reportErrorWarning(const char* warningString)
        {
            std::cerr << "CBulletDebugDrawer: " << warningString << std::endl;
        }

        void CBulletDebugDrawer::draw3dText(const btVector3& location,const char* textString)
        {
            irr::video::IVideoDriver *video = mIrrlicht->getVideoDriver();
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
    } // End NameSpace Engine
} // End NameSpace Kiaro
