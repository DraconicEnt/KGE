/**
 */

#ifndef _INCLUDE_CORE_ISERIALIZABLE_HPP_
#define _INCLUDE_CORE_ISERIALIZABLE_HPP_

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;

        class ISerializable
        {
            public:
                ISerializable(void) { }
                ISerializable(Support::CBitStream& in) { this->extractFrom(in); }
                virtual void writeTo(Support::CBitStream& out) const { }

                virtual void extractFrom(Support::CBitStream& in) { }

                virtual size_t getRequiredMemory(void) const { }
        };
    }
}
#endif // _INCLUDE_CORE_ISERIALIZABLE_HPP_
