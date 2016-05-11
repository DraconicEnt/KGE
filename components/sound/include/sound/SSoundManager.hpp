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
            // Private Members
            private:
                //! Pointer to the FMod sound system.
                FMOD::System* mFMod;

            public:
                CSoundSource* getSoundSource(const Support::String& filename);

            private:
                SSoundManager(void);
                ~SSoundManager(void);
        };
    } // End NameSpace Sound
} // End NameSpace Kiaro
