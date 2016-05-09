/**
 *  @file IDebugRenderer.hpp
 */

#ifndef _INCLUDE_PHYS_IDEBUGRENDERER_HPP_
#define _INCLUDE_PHYS_IDEBUGRENDERER_HPP_

#include <support/common.hpp>
#include <support/String.hpp>

#include <LinearMath/btIDebugDraw.h>

namespace Kiaro
{
    namespace Phys
    {
        class IDebugRenderer : public btIDebugDraw
        {
            // Public Methods
            public:
                virtual void drawLine(const Common::Vector3DF& from, const Common::Vector3DF& to, const Common::ColorRGBA& color) = 0;
                virtual void drawContactPoint(const Common::Vector3DF& pointOnB, const Common::Vector3DF& normalOnB, const Common::F32& distance, const Common::S32& lifetime, const Common::ColorRGBA& color) = 0;
                virtual void draw3dText(const Common::Vector3DF& location, const Support::String& text) = 0;

            // Private Methods
            private:
                void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
                void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,btScalar distance, Common::S32 lifeTime, const btVector3& color);
                void reportErrorWarning(const char* warningString);
                void draw3dText(const btVector3& location, const char* textString);

                void setDebugMode(Kiaro::Common::S32 debugMode);
                Common::S32 getDebugMode() const;
        };
    } // End NameSpace Phys
} // End NameSpace Kiaro
#endif

