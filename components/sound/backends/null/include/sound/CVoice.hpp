/**
 *  @file IVoice.hpp
 *  @brief Source file implementing the CVoice class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <sound/IVoice.hpp>

namespace Kiaro
{
    namespace Sound
    {
        class CVoice : public IVoice
        {
            // Public Methods
            public:
                void setPlaybackTimeMS(const Common::U32& ms);

                void setPositionVelocity(const Support::Vector3DF& position, const Support::Vector3DF& velocity);

                Support::Vector3DF getPosition(void);

                Support::Vector3DF getVelocity(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
