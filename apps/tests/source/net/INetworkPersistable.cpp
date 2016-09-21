/**
 *  @file TNetworkPersistable.cpp
 *  @brief Testing code for the INetworkPersistable class.
 */

#include <gtest/gtest.h>

#include <net/INetworkPersistable.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        class TestEntity : public Net::INetworkPersistable
        {
            public:
                size_t getRequiredMemory(void) const
                {
                    return 2;
                }
        };

        TEST(INetworkPersistable, PropertyLocations)
        {
            TestEntity networkedEntity;

            // A couple of networked values
            Common::F32 networkedFloat = 3.14f;
            Common::U32 networkedUInt = 1337;
            Support::String networkedString = "This is a string.";

            networkedEntity.addNetworkedProperty("float", networkedFloat);
            networkedEntity.addNetworkedProperty("uint", networkedUInt);
            networkedEntity.addNetworkedProperty("string", networkedString);

            // Read them back
            const Common::F32& readFloat = networkedEntity.getNetworkedPropertyValue<Common::F32>("float");
            const Common::U32& readUInt = networkedEntity.getNetworkedPropertyValue<Common::U32>("uint");
            const Support::String& readString = networkedEntity.getNetworkedPropertyValue<Support::String>("string");

            // Check raw values
            EXPECT_EQ(readFloat, networkedFloat);
            EXPECT_EQ(readUInt, networkedUInt);
            EXPECT_EQ(readString, networkedString);

            // Are they of the same address?
            EXPECT_EQ(&readFloat, &networkedFloat);
            EXPECT_EQ(&readUInt, &networkedUInt);
            EXPECT_EQ(&readString, &networkedString);
        }

        TEST(INetworkPersistable, IndirectPropertyWrite)
        {
            TestEntity networkedEntity;

            // A couple of networked values
            Common::F32 networkedFloat = 3.14f;
            Support::String networkedString = "This is a string.";

            networkedEntity.addNetworkedProperty("float", networkedFloat);
            networkedEntity.addNetworkedProperty("string", networkedString);

            // Set the value indirectly
            networkedFloat = 5.15f;
            networkedString = "Bollocks.";

            // Are they equal?
            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Common::F32>("float"), 5.15f);
            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Support::String>("string"), "Bollocks.");
        }

        TEST(INetworkPersistable, ReadTypes)
        {
            TestEntity networkedEntity;

            // A couple of networked values
            Common::F32 networkedFloat = 3.14f;
            Support::String networkedString = "This is a string.";

            networkedEntity.addNetworkedProperty("float", networkedFloat);
            networkedEntity.addNetworkedProperty("string", networkedString);

            EXPECT_NO_THROW(EXPECT_EQ(3.14f, networkedEntity.getNetworkedPropertyValue<Common::F32>("float")));
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U32>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U8>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::F64>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U64>("float"), std::runtime_error);

            EXPECT_NO_THROW(EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Support::String>("string"), "This is a string."));
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U32>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U8>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::F64>("float"), std::runtime_error);
            EXPECT_THROW(networkedEntity.getNetworkedPropertyValue<Common::U64>("float"), std::runtime_error);
        }

        TEST(INetworkPersistable, WriteTypes)
        {
            TestEntity networkedEntity;

            // A couple of networked values
            Common::F32 networkedFloat = 3.14f;
            networkedEntity.addNetworkedProperty("float", networkedFloat);

            EXPECT_NO_THROW(networkedEntity.setNetworkedPropertyValue<Common::F32>("float", 5.15f));
            EXPECT_THROW(networkedEntity.setNetworkedPropertyValue<Common::U32>("float", 15), std::runtime_error);

            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Common::F32>("float"), 5.15f);
            EXPECT_THROW(networkedEntity.setNetworkedPropertyValue<Common::U8>("float", 1), std::runtime_error);

            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Common::F32>("float"), 5.15f);
            EXPECT_THROW(networkedEntity.setNetworkedPropertyValue<Common::F64>("float", 6.19f), std::runtime_error);

            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Common::F32>("float"), 5.15f);
            EXPECT_THROW(networkedEntity.setNetworkedPropertyValue<Common::U64>("float", 1337), std::runtime_error);

            EXPECT_EQ(networkedEntity.getNetworkedPropertyValue<Common::F32>("float"), 5.15f);
        }

        TEST(INetworkPersistable, PackUnpack)
        {
            TestEntity serverEntity;
            TestEntity clientEntity;

            // A couple of networked values
            Common::F32 serverFloat = 3.14f;
            Common::U32 serverInt = 1337;

            serverEntity.addNetworkedProperty("float", serverFloat);
            serverEntity.addNetworkedProperty("uint", serverInt);

            // Now pack into a BitStream
            Support::CBitStream stream(256);
            serverEntity.packEverything(stream);

            Common::F32 clientFloat = 7.83f;
            Common::U32 clientInt = 1995;

            clientEntity.addNetworkedProperty("float", clientFloat);
            clientEntity.addNetworkedProperty("uint", clientInt);

            // Unpack into it
            stream.setPointer(0);
            clientEntity.unpack(stream);

            // Make sure the values are correct
            EXPECT_EQ(serverInt, clientEntity.getNetworkedPropertyValue<Common::U32>("uint"));
            EXPECT_EQ(serverFloat, clientEntity.getNetworkedPropertyValue<Common::F32>("float"));
        }
    } // End NameSpace Net
} // End NameSpace Kiaro


