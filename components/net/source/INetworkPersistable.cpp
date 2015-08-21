/**
 *  @file INetworkPersistable.cpp
 */

#include <net/INetworkPersistable.hpp>

namespace Kiaro
{
    namespace Net
    {
        INetworkPersistable::INetworkPersistable(void) { }

        template <typename propertyType>
        void INetworkPersistable::addNetworkedProperty(const Support::String& name, propertyType& propertyValue)
        {
            static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

            mNetworkedProperties[Common::string_hash(name)] = std::make_tuple(&propertyValue, PROPERTY_F32, 0);
        }

        template <typename propertyType>
        void INetworkPersistable::setNetworkedPropertyValue(const Support::String& name, const propertyType& newValue)
        {
            static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

            size_t mapIndex = Common::string_hash(name);
            Support::Tuple<void*, PROPERTY_TYPE, size_t> networkedPropertyInfo = mNetworkedProperties[mapIndex];

            // Is it the same type?
            if (std::get<1>(networkedPropertyInfo) != TypeIDResolver<propertyType>::value)
                throw std::logic_error("INetworkPersistable: Networked property type mismatch!");

            // Assign it
            propertyType& oldPropertyValue = *(reinterpret_cast<propertyType*>(std::get<0>(networkedPropertyInfo)));
            oldPropertyValue = newValue;

            // Add to the dirty properties
            if (mDirtyNetworkedProperties.count(mapIndex) == 0)
                mDirtyNetworkedProperties.insert(mDirtyNetworkedProperties.end(), mapIndex);
        }

        template <typename propertyType>
        const propertyType& INetworkPersistable::getNetworkedPropertyValue(const Support::String& name)
        {
            static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

            Support::Tuple<void*, PROPERTY_TYPE, size_t> networkedPropertyInfo = mNetworkedProperties[Common::string_hash(name)];

            // Is it the same type?
            if (std::get<1>(networkedPropertyInfo) != TypeIDResolver<propertyType>::value)
                throw std::logic_error("INetworkPersistable: Networked property type mismatch!");

            propertyType& returnValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
            return returnValue;
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
