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

#include <sound/CSoundSource.hpp>
#include <support/Console.hpp>

namespace Kiaro
{
    namespace Sound
    {
        CSoundSource::CSoundSource(const Support::String& filename)
        {

        }

        CSoundSource::~CSoundSource(void)
        {

        }

        IVoice* CSoundSource::play(void)
        {
            return nullptr;
        }
    } // End NameSpace Sound
} // End NameSpace Kiaro
