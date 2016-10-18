/**
 *  @file CRenderModel.cpp
 *  @brief Include file declaring the CRenderModel class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */


#ifndef _INCLUDE_CES_CRENDERMODEL_HPP_
#define _INCLUDE_CES_CRENDERMODEL_HPP_

#include <support/common.hpp>

#include <ces/IRenderable.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            /**
             *  @brief The CRenderModel component represents a model to be rendered from some on disk asset path. If the asset has already
             *  been loaded, then it will simply be reused from the in memory cache.
             */
            class CRenderModel : public IRenderable
            {
                private:
                    const Support::String mFilename;

                public:
                    CRenderModel(const Support::String& filename);
            };
        }
    }
}
#endif // _INCLUDE_CES_CRENDERMODEL_HPP_
