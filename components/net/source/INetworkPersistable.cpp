/**
 */

#include <net/INetworkPersistable.hpp>

namespace Kiaro
{
    namespace Net
    {
        template <>
        void INetworkPersistable::addNetworkedProperty(const Support::String& name, Common::F32& propertyValue)
        {
            mNetworkedProperties[Common::string_hash(name)] = std::make_tuple(&propertyValue, PROPERTY_F32, 0);
        }

        template <>
        void INetworkPersistable::addNetworkedProperty(const Support::String& name, Common::F64& propertyValue)
        {
            mNetworkedProperties[Common::string_hash(name)] = std::make_tuple(&propertyValue, PROPERTY_F64, 0);
        }

        template <>
        void INetworkPersistable::addNetworkedProperty(const Support::String& name, Common::U32& propertyValue)
        {
            mNetworkedProperties[Common::string_hash(name)] = std::make_tuple(&propertyValue, PROPERTY_U32, 0);
        }

        template <>
        void INetworkPersistable::addNetworkedProperty(const Support::String& name, Common::U64& propertyValue)
        {
            mNetworkedProperties[Common::string_hash(name)] = std::make_tuple(&propertyValue, PROPERTY_U64, 0);
        }

        void INetworkPersistable::packDeltas(Support::CBitStream& out)
        {
            out.write<Common::U32>(mDirtyNetworkedProperties.size());

            for (auto it = mDirtyNetworkedProperties.begin(); it != mDirtyNetworkedProperties.end(); it++)
            {
                const size_t& propertyHash = *it;
                const Support::Tuple<void*, PROPERTY_TYPE, size_t>& networkedPropertyInfo = mNetworkedProperties[propertyHash];

                this->packProperty(out, propertyHash, networkedPropertyInfo);
            }

            mDirtyNetworkedProperties.clear();
        }

        void INetworkPersistable::packEverything(Support::CBitStream& out) const
        {
            out.write<Common::U32>(mNetworkedProperties.size());

            for (auto it = mNetworkedProperties.begin(); it != mNetworkedProperties.end(); it++)
            {
                std::pair<size_t, Support::Tuple<void*, PROPERTY_TYPE, size_t>> networkedPropertyInfo = *it;
                this->packProperty(out, networkedPropertyInfo.first, networkedPropertyInfo.second);
            }
        }

        void INetworkPersistable::packProperty(Support::CBitStream& out, const size_t& propertyHash, const Support::Tuple<void*, PROPERTY_TYPE, size_t>& property) const
        {
            // Write the property hash
            // FIXME (Robert MacGregor#9): size_t Differences will cause breakage when networking two systems that have different sizeof(size_t)
            out.write(propertyHash);

            // Pack each type accordingly
            switch(std::get<1>(property))
            {
                case PROPERTY_F32:
                {
                    const Common::F32& value = *reinterpret_cast<Common::F32*>(std::get<0>(property));
                    out.write<Common::F32>(value);
                    break;
                }

                case PROPERTY_F64:
                {
                    const Common::F32& value = *reinterpret_cast<Common::F64*>(std::get<0>(property));
                    out.write<Common::F64>(value);
                    break;
                }

                case PROPERTY_U32:
                {
                    const Common::F32& value = *reinterpret_cast<Common::U32*>(std::get<0>(property));
                    out.write<Common::U32>(value);
                    break;
                }

                case PROPERTY_U64:
                {
                    const Common::F32& value = *reinterpret_cast<Common::U32*>(std::get<0>(property));
                    out.write<Common::U64>(value);
                    break;
                }

                default:
                {
                    throw std::domain_error("INetworkPersistable: Encountered unknown type in packDeltas!");
                    break;
                }
            }
        }

        void INetworkPersistable::unpack(Support::CBitStream& in)
        {
            // How many properties are there to unpack?
            const Common::U32& propertyCount = *in.top<Common::U32>();
            in.pop<Common::U32>();

            // Unpack that many properties: If the payload was crafted to have wrong numbers, then the bit stream will throw underflow exceptions
            for (Common::U32 iteration = 0; iteration < propertyCount; iteration++)
            {
                const PROPERTY_TYPE& propertyType = *in.top<PROPERTY_TYPE>();
                in.pop<PROPERTY_TYPE>();

                const size_t& propertyHash = *in.top<size_t>();
                in.pop<size_t>();

                // Do we have such a property?
                if (mNetworkedProperties.count(propertyHash) == 0)
                    throw std::domain_error("INetworkPersisable: Encountered unknown property in unpack!");

                const Support::Tuple<void*, PROPERTY_TYPE, size_t>& propertyInformation = mNetworkedProperties[propertyHash];

                switch(propertyType)
                {
                    case PROPERTY_F32:
                    {
                        Common::F32& out = *reinterpret_cast<Common::F32*>(std::get<0>(propertyInformation));
                        out = *in.top<Common::F32>();
                        in.pop<Common::F32>();

                        break;
                    }

                    case PROPERTY_F64:
                    {
                        Common::F64& out = *reinterpret_cast<Common::F64*>(std::get<0>(propertyInformation));
                        out = *in.top<Common::F64>();
                        in.pop<Common::F64>();

                        break;
                    }

                    case PROPERTY_U32:
                    {
                        Common::U32& out = *reinterpret_cast<Common::U32*>(std::get<0>(propertyInformation));
                        out = *in.top<Common::U32>();
                        in.pop<Common::U32>();

                        break;
                    }

                    case PROPERTY_U64:
                    {
                        Common::U64& out = *reinterpret_cast<Common::U64*>(std::get<0>(propertyInformation));
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
