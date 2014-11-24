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

        Kiaro::Common::C8 *BitStream::readString(const size_t &outStringLength, const bool &shouldMemcpy)
        {
        	// If the Length if zero, try to figure out where we should stop, then
        	// FIXME: Go out until maximum arb string length
        	//for (size_t iteration = mDataPointer; iteration < mDataLength; iteration++)
			//	if (mData[iteration] == 0x00) // Check for NULL termination
            //        return (Kiaro::Common::C8*)read<Kiaro::Common::C8>(iteration - mDataLength, shouldMemcpy);

            //return (Kiaro::Common::C8*)read(outStringLength, shouldMemcpy);
        }

        void *BitStream::getBasePointer(void)
        {
            return mData;
        }

        void *BitStream::raw(const bool &shouldMemcpy)
        {
            //return read(mDataPointer, shouldMemcpy);
            return NULL;
        }

        size_t BitStream::length(void)
        {
            return mDataLength;
        }

        template <>
        Kiaro::Common::Vector3DF &BitStream::read<Kiaro::Common::Vector3DF>(const bool &shouldMemcpy)
        {
            Kiaro::Common::Vector3DF *result = new Kiaro::Common::Vector3DF;
            result->Z = read<Kiaro::Common::F32>();
            result->Y = read<Kiaro::Common::F32>();
            result->X = read<Kiaro::Common::F32>();
            return *result;
        }
    } // End Namespace Support
} // End namespace Kiaro
