/**
 *  @file IDebugRenderer.cpp
 *  @brief Source file implementing the IDebugRenderer interface class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <phys/IDebugRenderer.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Phys
    {
        void IDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
        {
            this->drawLine(Support::Vector3DF(from.x(), from.y(), from.z()), Support::Vector3DF(to.x(), to.y(), to.z()), Support::ColorRGBA(255, color.x(), color.y(), color.z()));
        }

        void IDebugRenderer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, Common::S32 lifeTime, const btVector3& color)
        {
            this->drawContactPoint(Support::Vector3DF(pointOnB.x(), pointOnB.y(), pointOnB.z()), Support::Vector3DF(normalOnB.x(), normalOnB.y(), normalOnB.z()), distance, lifeTime, Support::ColorRGBA(255, color.x(), color.y(), color.z()));
        }

        void IDebugRenderer::reportErrorWarning(const char* warningString)
        {
            CONSOLE_WARNING(warningString);
        }

        void IDebugRenderer::draw3dText(const btVector3& location, const char* textString)
        {
            this->draw3dText(Support::Vector3DF(location.x(), location.y(), location.z()), textString);
        }

        void IDebugRenderer::setDebugMode(Kiaro::Common::S32 debugMode)
        {
        }

        Common::S32 getDebugMode()
        {
            return 0;
        }
    } // End NameSpace Phys
} // End NameSpace Kiaro
