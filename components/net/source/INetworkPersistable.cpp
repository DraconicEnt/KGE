/**
 *  @file INetworkPersistable.cpp
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
                const size_t& propertyHash = *it;
                const std::pair<void*, PROPERTY_TYPE>& networkedPropertyInfo = mNetworkedProperties[propertyHash];

                this->packProperty(out, propertyHash, networkedPropertyInfo);
            }

            mDirtyNetworkedProperties.clear();
        }

        void INetworkPersistable::packEverything(Support::CBitStream& out) const
        {
            for (auto it = mNetworkedProperties.begin(); it != mNetworkedProperties.end(); it++)
            {
                std::pair<size_t, std::pair<void*, PROPERTY_TYPE>> networkedPropertyInfo = *it;
                this->packProperty(out, networkedPropertyInfo.first, networkedPropertyInfo.second);
            }

            out.write<Common::U32>(mNetworkedProperties.size());
        }

        void INetworkPersistable::packProperty(Support::CBitStream& out, const size_t& propertyHash, const std::pair<void*, PROPERTY_TYPE>& property) const
        {
            // Write the property hash
            // FIXME (Robert MacGregor#9): size_t Differences will cause breakage when networking two systems that have different sizeof(size_t)

            // Pack each type accordingly
            switch(property.second)
            {
                case PROPERTY_F32:
                {
                    const Common::F32& value = *reinterpret_cast<Common::F32*>(property.first);
                    out.write<Common::F32>(value);
                    break;
                }

                case PROPERTY_F64:
                {
                    const Common::F32& value = *reinterpret_cast<Common::F64*>(property.first);
                    out.write<Common::F64>(value);
                    break;
                }

                case PROPERTY_U32:
                {
                    const Common::U32& value = *reinterpret_cast<Common::U32*>(property.first);
                    out.write<Common::U32>(value);
                    break;
                }

                case PROPERTY_U64:
                {
                    const Common::F32& value = *reinterpret_cast<Common::U32*>(property.first);
                    out.write<Common::U64>(value);
                    break;
                }

                default:
                {
                    throw std::domain_error("INetworkPersistable: Encountered unknown type in packDeltas!");
                    break;
                }
            }

            out.write<size_t>(propertyHash);
        }

        void INetworkPersistable::unpack(Support::CBitStream& in)
        {
            // How many properties are there to unpack?
            const Common::U32& propertyCount = *in.top<Common::U32>();
            in.pop<Common::U32>();

            // Unpack that many properties: If the payload was crafted to have wrong numbers, then the bit stream will throw underflow exceptions
            for (Common::U32 iteration = 0; iteration < propertyCount; iteration++)
            {
                // TODO (Robert MacGregor#9): Determine what to do when properties don't exist and are being unpacked here
              //  const PROPERTY_TYPE& propertyType = *in.top<PROPERTY_TYPE>();
              //  in.pop<PROPERTY_TYPE>();

                const size_t& propertyHash = *in.top<size_t>();
                in.pop<size_t>();

                // Do we have such a property?
                if (mNetworkedProperties.count(propertyHash) == 0)
                {
                    Support::String exceptionMessage = "INetworkPersisable: Encountered unknown property in unpack! HashCode: ";
                    exceptionMessage += propertyHash;

                    throw std::domain_error(exceptionMessage);
                }

                const std::pair<void*, PROPERTY_TYPE>& propertyInformation = mNetworkedProperties[propertyHash];

                switch(propertyInformation.second)
                {
                    case PROPERTY_F32:
                    {
                        Common::F32& out = *reinterpret_cast<Common::F32*>(propertyInformation.first);
                        out = *in.top<Common::F32>();
                        in.pop<Common::F32>();

                        break;
                    }

                    case PROPERTY_F64:
                    {
                        Common::F64& out = *reinterpret_cast<Common::F64*>(propertyInformation.first);
                        out = *in.top<Common::F64>();
                        in.pop<Common::F64>();

                        break;
                    }

                    case PROPERTY_U32:
                    {
                        Common::U32& out = *reinterpret_cast<Common::U32*>(propertyInformation.first);
                        out = *in.top<Common::U32>();
                        in.pop<Common::U32>();

                        break;
                    }

                    case PROPERTY_U64:
                    {
                        Common::U64& out = *reinterpret_cast<Common::U64*>(propertyInformation.first);
                        out = *in.top<Common::U64>();
                        in.pop<Common::U64>();

                        break;
                    }

                    default:
                    {
                        throw std::domain_error("INetworkPersistable: Encountered unknown type in unpack!");
                        break;
                    }
                }
            }
        }
    } // End NameSpace Net
} // End NameSpace Kiaro
