/**
 *  @file CBitStream.h
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_CBITSTREAM_H_
#define _INCLUDE_KIARO_SUPPORT_CBITSTREAM_H_

#include <type_traits>  // std::is_pointer
#include <exception>

#include <physfs.h>

#include <support/support.hpp>
#include <support/common.hpp>

//#include <core/ISerializable.hpp>
namespace Kiaro
{
    namespace Support
    {
        class ISerializable;

        class CBitStream
        {
            public:
                CBitStream(ISerializable* in);

                CBitStream(void* initializer, const size_t& initializerLength);

                CBitStream(const size_t& sizeInBytes, const void* initializer = NULL, size_t initializerLength = 0);

                //! Standard destructor.
                ~CBitStream(void);

                template <typename inType>
                void write(inType input)
                {
                    if (mPointer >= mTotalSize || mTotalSize - mPointer < sizeof(inType))
                        throw std::overflow_error("Stack Overflow");

                    inType &output = *(inType*)(&mMemoryBlock[mPointer]);
                    output = input;

                    mPointer += sizeof(inType);
                }

                void write(const ISerializable* in);

                template <typename type>
                void pop(void)
                {
                    if (mPointer <= 0 || sizeof(type) > mPointer)
                        throw std::underflow_error("Stack Underflow");

                    mPointer -= sizeof(type);
                }

                template <typename outType>
                outType* top(void) const
                {
                    if (mPointer <= 0 || sizeof(outType) > mPointer)
                        throw std::underflow_error("Stack Underflow");

                    return (outType *)&mMemoryBlock[mPointer - sizeof(outType)];
                }

                template <typename outType>
                outType* extract(void)
                {
                    return new outType(*this);
                }

                bool isFull(void) const;

                bool isEmpty(void) const;

                void resize(const size_t &newSize);

                template <typename inType>
                friend CBitStream& operator <<(CBitStream& stack, inType input)
                {
                    stack.write(input);
                    return stack;
                }

                template <typename outType>
                friend CBitStream& operator >>(CBitStream& stack, outType& output)
                {
                    output = *stack.top<outType>();
                    stack.pop<outType>();

                    return stack;
                }

                template <typename serializableType>
                friend CBitStream& operator >>(CBitStream& stack, serializableType*& output)
                {
                    output = stack.extract<serializableType>();
                    return stack;
                }

                size_t getWrittenLength(void);
                void* getBlock(void);
                void setPointer(const size_t& pointer);

            private:
                //! A pointer to the start of the contiguous memory block.
                Common::U8* mMemoryBlock;

                //! The number of elements currently in the stack.
                int mPointer;

                size_t mTotalSize;

                bool mOwnsMemoryBlock;
        };

        class ISerializable
        {
            public:
                ISerializable(void) { }
                ISerializable(Support::CBitStream& in) { this->unpack(in); }

                virtual void packEverything(Support::CBitStream& out) const = 0;

                virtual void unpack(Support::CBitStream& in) { }

                virtual size_t getRequiredMemory(void) const { }
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_BITSTREAM_H_
