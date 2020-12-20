/**
 *  @file IDebugRenderer.hpp
 *  @brief INclude file declaring the IDebugRenderer interface class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_PHYS_IDEBUGRENDERER_HPP_
#define _INCLUDE_PHYS_IDEBUGRENDERER_HPP_

#include <support/common.hpp>
#include <support/String.hpp>
#include <support/types.hpp>

#include <bullet/LinearMath/btIDebugDraw.h>

namespace Kiaro
{
    namespace Phys
    {
        class IDebugRenderer : public btIDebugDraw
        {
            public:
                virtual void drawLine(const Support::Vector3DF& from, const Support::Vector3DF& to, const Support::ColorRGBA& color) = 0;
                virtual void drawContactPoint(const Support::Vector3DF& pointOnB, const Support::Vector3DF& normalOnB, const Common::F32& distance, const Common::S32& lifetime, const Support::ColorRGBA& color) = 0;
                virtual void draw3dText(const Support::Vector3DF& location, const Support::String& text) = 0;

            private:
                void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
                void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, Common::S32 lifeTime, const btVector3& color);
                void reportErrorWarning(const char* warningString);
                void draw3dText(const btVector3& location, const char* textString);

                void setDebugMode(Kiaro::Common::S32 debugMode);
                Common::S32 getDebugMode() const;
        };
    } // End NameSpace Phys
} // End NameSpace Kiaro
#endif
