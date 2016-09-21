/**
 *  @file INetworkPersistable.cpp
 *  @brief Source file implementing the INetworkPersistable interface class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <net/INetworkPersistable.hpp>

namespace Kiaro
{
    namespace Net
    {
        INetworkPersistable::INetworkPersistable(void) { }

        void INetworkPersistable::packDeltas(Support::CBitStream& out)
        {
            out.write<Common::U32>(mDirtyNetworkedProperties.size());

            for (auto it = mDirtyNetworkedProperties.begin(); it != mDirtyNetworkedProperties.end(); it++)
            {
                Support::String name = *it;

                const std::pair<void*, Support::PROPERTY_TYPE>& networkedPropertyInfo = mNetworkedProperties[name];
                this->packProperty(out, name, networkedPropertyInfo);
            }

            mDirtyNetworkedProperties.clear();
        }

        void INetworkPersistable::packEverything(Support::CBitStream& out) const
        {
            out.write<Common::U32>(mNetworkedProperties.size());

            for (auto it = mNetworkedProperties.begin(); it != mNetworkedProperties.end(); it++)
            {
                std::pair<Support::String, std::pair<void*, Support::PROPERTY_TYPE>> networkedPropertyInfo = *it;
                this->packProperty(out, networkedPropertyInfo.first, networkedPropertyInfo.second);
            }
        }

        void INetworkPersistable::packProperty(Support::CBitStream& out, const Support::String& propertyName, const std::pair<void*, Support::PROPERTY_TYPE>& property) const
        {
            // Write the property hash
            // FIXME (Robert MacGregor#9): size_t Differences will cause breakage when networking two systems that have different sizeof(size_t)
            out.writeString(propertyName);

            // Pack each type accordingly
            switch(property.second)
            {
                case Support::PROPERTY_F32:
                {
                    const Common::F32& value = *reinterpret_cast<Common::F32*>(property.first);
                    out.write<Common::F32>(value);
                    break;
                }

                case Support::PROPERTY_F64:
                {
                    const Common::F64& value = *reinterpret_cast<Common::F64*>(property.first);
                    out.write<Common::F64>(value);
                    break;
                }

                case Support::PROPERTY_U32:
                {
                    const Common::U32& value = *reinterpret_cast<Common::U32*>(property.first);
                    out.write<Common::U32>(value);
                    break;
                }

                case Support::PROPERTY_U64:
                {
                    const Common::U64& value = *reinterpret_cast<Common::U64*>(property.first);
                    out.write<Common::U64>(value);
                    break;
                }

                case Support::PROPERTY_STRING:
                {
                    const Support::String& value = *reinterpret_cast<Support::String*>(property.first);
                    out.writeString(value);
                    break;
                }

                case Support::PROPERTY_VECTOR3DF:
                {
                    const Common::Vector3DF& value = *reinterpret_cast<Common::Vector3DF*>(property.first);
                    out.write<Common::Vector3DF>(value);
                    break;
                }

                default:
                    throw std::domain_error("INetworkPersistable: Encountered unknown type in packDeltas!");
            }
        }

        void INetworkPersistable::unpack(Support::CBitStream& in)
        {
            // How many properties are there to unpack?
            const Common::U32& propertyCount = in.pop<Common::U32>();

            // Unpack that many properties: If the payload was crafted to have wrong numbers, then the bit stream will throw underflow exceptions
            for (Common::U32 iteration = 0; iteration < propertyCount; iteration++)
            {
                // TODO (Robert MacGregor#9): Determine what to do when properties don't exist and are being unpacked here
                //  const PROPERTY_TYPE& propertyType = *in.top<PROPERTY_TYPE>();
                //  in.pop<PROPERTY_TYPE>();
                const Common::C8* propertyName = in.popString();

                // Do we have such a property?
                if (mNetworkedProperties.count(propertyName) == 0)
                {
                    Support::String exceptionMessage = "INetworkPersisable: Encountered unknown property in unpack! Name: ";
                    exceptionMessage += propertyName;
                    throw std::domain_error(exceptionMessage);
                }

                const std::pair<void*, Support::PROPERTY_TYPE>& propertyInformation = mNetworkedProperties[propertyName];

                switch(propertyInformation.second)
                {
                    case Support::PROPERTY_F32:
                    {
                        Common::F32& out = *reinterpret_cast<Common::F32*>(propertyInformation.first);
                        out = in.pop<Common::F32>();
                        break;
                    }

                    case Support::PROPERTY_F64:
                    {
                        Common::F64& out = *reinterpret_cast<Common::F64*>(propertyInformation.first);
                        out = in.pop<Common::F64>();
                        break;
                    }

                    case Support::PROPERTY_U32:
                    {
                        Common::U32& out = *reinterpret_cast<Common::U32*>(propertyInformation.first);
                        out = in.pop<Common::U32>();
                        break;
                    }

                    case Support::PROPERTY_U64:
                    {
                        Common::U64& out = *reinterpret_cast<Common::U64*>(propertyInformation.first);
                        out = in.pop<Common::U64>();
                        break;
                    }

                    case Support::PROPERTY_STRING:
                    {
                        Support::String& out = *reinterpret_cast<Support::String*>(propertyInformation.first);
                        out = in.popString();
                        break;
                    }

                    case Support::PROPERTY_VECTOR3DF:
                    {
                        Common::Vector3DF& out = *reinterpret_cast<Common::Vector3DF*>(propertyInformation.first);
                        out = in.pop<Common::Vector3DF>();
                        break;
                    }

                    default:
                        throw std::domain_error("INetworkPersistable: Encountered unknown type in unpack!");
                }
            }
        }
    } // End NameSpace Net
} // End NameSpace Kiaro
