/**
 *  @file SSoundManager.hpp
 */

#include <fmod.hpp>

#include <support/String.hpp>
#include <sound/CSoundSource.hpp>

namespace Kiaro
{
    namespace Sound
    {
        /**
         *  @brief Singleton class for the sound management programming of the engine.
         */
        class SSoundManager
        {
            // Public Members
            public:
                /**
                 *  @brief Gets a sound source associated with the given filename.
                 *  @param filename The name of the sound file to source from
                 *  @return A pointer to the created sound source.
                 */
                CSoundSource* getSoundSource(const Support::String& filename);

            // Private Members
            private:
                //! Pointer to the FMod sound system.
                FMOD::System* mFMod;

            private:
                /**
                 *  @brief Parameter-less constructor.
                 */
                SSoundManager(void);

                /**
                 *  @brief Standard destructor.
                 */
                ~SSoundManager(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
