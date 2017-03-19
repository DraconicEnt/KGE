/**
 *  @file CSoundSource.cpp
 *  @brief Source file implementing the CSoundSource class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
#define _INCLUDE_SOUND_CSOUNDSOURCE_HPP_

#include <sound/ISoundSource.hpp>

namespace Kiaro
{
    namespace Sound
    {
        class CSoundSource : public ISoundSource
        {
            public:
                CSoundSource(const Support::String& filename);
                ~CSoundSource(void);

                IVoice* play(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro

#endif // _INCLUDE_SOUND_CSOUNDSOURCE_HPP_
