namespace Kiaro
{
    namespace Support
    {
        class CBitStream;

        class ISerializable
        {
            // Public Methods
            public:

                virtual void packData(Kiaro::Support::CBitStream &out) = 0;
                virtual void unpackData(Kiaro::Support::CBitStream &in) = 0;
        };
    }
}
