/**
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
