namespace Kiaro
{
    namespace Support
    {
        class CBitStream;

        class ISerializable
        {
            public:
                ISerializable(void) { }
                ISerializable(Support::CBitStream &in) { this->extractFrom(in); }
                virtual void writeTo(Support::CBitStream &out) const = 0;

                virtual void extractFrom(Support::CBitStream &in) { }

                virtual size_t getRequiredMemory(void) = 0;
        };
    }
}
