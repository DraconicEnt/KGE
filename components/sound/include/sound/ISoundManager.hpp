/**
 *  @file SSoundManager.hpp
 *  @brief Include file declaring the SSoundManager singleton class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SOUND_ISOUNDMANAGER_HPP_
#define _INCLUDE_SOUND_ISOUNDMANAGER_HPP_

#include <support/String.hpp>
#include <support/UnorderedMap.hpp>
#include <support/ISingleton.hpp>
#include <support/types.hpp>

#include <sound/ISoundSource.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief Singleton class for the sound management programming of the engine. This singleton provides loading and memory caching
         *  of sound resources as well as asynchronous loading of said sound resources.
         */
        class ISoundManager
        {
            // Private Members
            private:
                //! A map of all loaded sound resources.
                Support::UnorderedMap<Support::String, ISoundSource*> mSoundRegistry;

            // Public Methods
            public:
                /**
                 *  @brief Gets a sound source associated with the given filename.
                 *  @param filename The name of the sound file to source from
                 *  @return A pointer to the created sound source.
                 */
                virtual ISoundSource* getSoundSource(const Support::String& filename) = 0;

                /**
                 *  @brief Performs any update logic that may be necessary to drive the backend sound systems.
                 */
                virtual void update(void) = 0;
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro

#endif // _INCLUDE_SOUND_ISOUNDMANAGER_HPP_
