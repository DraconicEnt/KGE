/**
 *  @file BitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <support/BitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        BitStream::BitStream(Kiaro::Common::U8 *initialData, size_t initialDataLength, size_t initialDataIndex) :
                             mIsManagingMemory(true), mDataPointer(initialDataIndex)
        {
            if (initialData == NULL)
                mDataLength = 0;
            else
                mDataLength = initialDataLength;

            mIsManagingMemory = false;
            mData = initialData;
        }

       // BitStream::BitStream(const Kiaro::Common::U8 *initialData, size_t initialDataLength, size_t initialDataIndex)
       // {
        //    BitStream((Kiaro::Common::U8*)initialData);
        //    mIsManagingMemory = false; // NOTE (Robert MacGregor#1): Gets Overwritten by the Constructor Call Above
       // }

        BitStream::BitStream(size_t initialDataLength) : mIsManagingMemory(true), mData(new Kiaro::Common::U8[initialDataLength]),
        mDataPointer(0), mDataLength(initialDataLength)
        {

        }

        BitStream::~BitStream(void)
        {
            if (mIsManagingMemory)
                delete mData;
        }

        void BitStream::write(const void *inData, const size_t &inDataLength)
        {
            // Check to see if we have enough room to write this data
            if (mDataLength < mDataPointer + inDataLength)
            {
                Kiaro::Common::U32 newDataLength = mDataLength + inDataLength;
                Kiaro::Common::U8 *newMemory = new Kiaro::Common::U8[newDataLength];

                memcpy(newMemory, mData, mDataLength);
                mDataLength = newDataLength;

                if (mData && mIsManagingMemory)
                    delete mData;

                mData = newMemory;
                mIsManagingMemory = true;
            }

            memcpy(&mData[mDataPointer], inData, inDataLength);
            mDataPointer += inDataLength; // NOTE (Robert MacGregor#9): Preserves the offset
        }

        void BitStream::writeF32(const Kiaro::Common::F32 &inData)
        {
            write(&inData, sizeof(Kiaro::Common::F32));
        }

        void BitStream::writeBool(const bool &inData)
        {
            write(&inData, sizeof(bool));
        }

        void BitStream::writeU8(const Kiaro::Common::U8 &inData)
        {
            write(&inData, sizeof(Kiaro::Common::U8));
        }

        void BitStream::writeU32(const Kiaro::Common::U32 &inData)
        {
            write(&inData, sizeof(Kiaro::Common::U32));
        }

        void BitStream::writeString(const std::string &inString)
        {
            write(inString.c_str(), inString.length() + 1);
        }

        void *BitStream::read(const size_t &outDataLength, const bool &shouldMemcpy)
        {
            if (outDataLength > mDataPointer)
                throw std::out_of_range("BitStream attempted to read out of range!");
            else
                mDataPointer -= outDataLength;

            if (shouldMemcpy)
            {
                void *memory = (void*)new Kiaro::Common::U8[outDataLength];
                memcpy(memory, &mData[mDataPointer], outDataLength);
                return memory;
            }

            return (void*)&mData[mDataPointer];
        }

        Kiaro::Common::F32 BitStream::readF32(const bool &should_memcpy)
        {
            return *(Kiaro::Common::F32*)read(sizeof(Kiaro::Common::F32), should_memcpy);
        }

        bool BitStream::readBool(const bool &shouldMemcpy)
        {
            return *(bool*)read(sizeof(bool), shouldMemcpy);
        }

        Kiaro::Common::U8 BitStream::readU8(const bool &shouldMemcpy)
        {
            return *(Kiaro::Common::U8*)read(sizeof(Kiaro::Common::U8), shouldMemcpy);
        }

        Kiaro::Common::U32 BitStream::readU32(const bool &shouldMemcpy)
        {
            return *(Kiaro::Common::U32*)read(sizeof(Kiaro::Common::U32), shouldMemcpy);
        }

        Kiaro::Common::C8 *BitStream::readString(const size_t &outStringLength, const bool &shouldMemcpy)
        {
        	// If the Length if zero, try to figure out where we should stop, then
        	// FIXME: Go out until maximum arb string length
        	for (size_t iteration = mDataPointer; iteration < mDataLength; iteration++)
				if (mData[iteration] == 0x00) // Check for NULL termination
                    return (Kiaro::Common::C8*)read(iteration - mDataLength, shouldMemcpy);

            return (Kiaro::Common::C8*)read(outStringLength, shouldMemcpy);
        }

        void *BitStream::raw(const bool &shouldMemcpy)
        {
            return read(mDataPointer, shouldMemcpy);
        }

        size_t BitStream::length(void)
        {
            return mDataLength;
        }
    } // End Namespace Support
} // End namespace Kiaro
