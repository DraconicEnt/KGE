/**
 *  @file IDebugRenderer.cpp
 */

#include <phys/IDebugRenderer.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Phys
    {
        void IDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
        {
            this->drawLine(Common::Vector3DF(from.x(), from.y(), from.z()), Common::Vector3DF(to.x(), to.y(), to.z()), Common::ColorRGBA(255, color.x(), color.y(), color.z()));
        }

        void IDebugRenderer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, Common::S32 lifeTime, const btVector3& color)
        {
            this->drawContactPoint(Common::Vector3DF(pointOnB.x(), pointOnB.y(), pointOnB.z()), Common::Vector3DF(normalOnB.x(), normalOnB.y(), normalOnB.z()), distance, lifeTime, Common::ColorRGBA(255, color.x(), color.y(), color.z()));
        }

        void IDebugRenderer::reportErrorWarning(const char* warningString)
        {
            CONSOLE_WARNING(warningString);
        }

        void IDebugRenderer::draw3dText(const btVector3& location, const char* textString)
        {
            this->draw3dText(Common::Vector3DF(location.x(), location.y(), location.z()), textString);
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
