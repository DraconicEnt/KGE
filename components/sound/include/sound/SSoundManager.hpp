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
                static SSoundManager* getPointer(void);
                static void destroy(void);

                /**
                 *  @brief Gets a sound source associated with the given filename.
                 *  @param filename The name of the sound file to source from
                 *  @return A pointer to the created sound source.
                 */
                CSoundSource* getSoundSource(const Support::String& filename);

                void update(void);

            // Private Members
            private:
                //! Pointer to the FMod sound system.
                FMOD::System* mFMod;

                static FMOD_RESULT fmodOpen(const char* name, unsigned int* filesize, void** handle, void* userdata);
                static FMOD_RESULT fmodClose(void* handle, void* userdata);
                static FMOD_RESULT fmodRead(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata);
                static FMOD_RESULT fmodSeek(void* handle, unsigned int pos, void *userdata);

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
