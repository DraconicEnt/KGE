/**
 *  @file TNetworkPersistable.cpp
 *  @brief Testing code for the INetworkPersistable class.
 */

#if ENGINE_TESTS>0

#include <gtest/gtest.h>

#include <net/INetworkPersistable.hpp>

namespace Kiaro
{
    namespace Net
    {
        TEST(INetworkPersistable, PropertyReadWrite)
        {
            Net::INetworkPersistable networkedEntity;

            // A couple of networked values
            Common::F32 networkedFloat = 3.14f;
            Common::U32 networkedUInt = 1337;

            networkedEntity.addNetworkedProperty("float", networkedFloat);
            networkedEntity.addNetworkedProperty("uint", networkedUInt);
        }
    } // End NameSpace Net
} // End NameSpace Kiaro
#endif


