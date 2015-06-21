/**
 *  @file SSettingsRegistry.cpp
 */

#include <core/SSettingsRegistry.hpp>

namespace Kiaro
{
    namespace Core
    {
        static SSettingsRegistry* sInstance = NULL;

        SSettingsRegistry* SSettingsRegistry::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SSettingsRegistry;

            return sInstance;
        }

        void SSettingsRegistry::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = NULL;
            }
        }

        SSettingsRegistry::SSettingsRegistry(void)
        {

        }

        SSettingsRegistry::~SSettingsRegistry(void)
        {
            // Make sure we clear the heap elements
            for (auto it = mStoredProperties.begin(); it != mStoredProperties.end(); it++)
                free(it->second.first);
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
