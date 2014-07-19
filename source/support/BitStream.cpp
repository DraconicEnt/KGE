/**
 *  @file BitStream.cpp
 *  @brief Source file defining the functions for manipulating a BitStream.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <support/BitStream.h>

namespace Kiaro
{
    namespace Support
    {
        BitStream::BitStream(Kiaro::Common::U8 *initial_data_array, Kiaro::Common::U32 initial_data_length, Kiaro::Common::U32 initial_data_index) :
                             is_managing_memory(true), current_data_index(initial_data_index)
        {
            if (initial_data_array == NULL)
                this->current_data_length = 0;
            else
                this->current_data_length = initial_data_length;

            this->is_managing_memory = true;
            this->current_data_array = initial_data_array;
        }

        BitStream::BitStream(const Kiaro::Common::U8 *initial_data_array, Kiaro::Common::U32 initial_data_length, Kiaro::Common::U32 initial_data_index)
        {
            BitStream((Kiaro::Common::U8*)initial_data_array);
            this->is_managing_memory = false; // NOTE (Robert MacGregor#1): Gets Overwritten by the Constructor Call Above
        }

        BitStream::BitStream(Kiaro::Common::U32 initial_data_length) : is_managing_memory(true), current_data_array(new Kiaro::Common::U8[initial_data_length]),
                             current_data_index(0), current_data_length(initial_data_length)
        {

        }

        BitStream::~BitStream(void)
        {
            if (this->is_managing_memory)
                delete this->current_data_array;
        }

        void BitStream::Write(void *in_data, Kiaro::Common::U32 in_data_length)
        {
            // Check to see if we have enough room to write this data
            if (this->current_data_length < this->current_data_index + in_data_length)
            {
                Kiaro::Common::U32 new_data_length = this->current_data_length + in_data_length;
                Kiaro::Common::U8 *new_memory = new Kiaro::Common::U8[new_data_length];
                //Common::u8 *new_memory = new Common::u8[new_data_length];

                memcpy(new_memory, this->current_data_array, this->current_data_length);
                this->current_data_length = new_data_length;

                if (this->current_data_array != 0x00 && this->is_managing_memory)
                    delete this->current_data_array;

                this->current_data_array = new_memory;
                this->is_managing_memory = true;
            }

            memcpy(&this->current_data_array[current_data_index], in_data, in_data_length);
            this->current_data_index += in_data_length; // NOTE (Robert MacGregor#9): Preserves the offset
        }

        void BitStream::WriteF32(Kiaro::Common::F32 in_data)
        {
            Write(&in_data, sizeof(Kiaro::Common::F32));
        }

        void BitStream::WriteBool(bool in_data)
        {
            Write(&in_data, sizeof(bool));
        }

        void BitStream::WriteU8(Kiaro::Common::U8 in_data)
        {
            Write(&in_data, sizeof(Common::U8));
        }

        void BitStream::WriteU32(Kiaro::Common::U32 in_data)
        {
            Write(&in_data, sizeof(Common::U32));
        }

        void BitStream::WriteString(Kiaro::Common::C8 *in_string)
        {
            Write(&in_string[0], strlen(in_string) + 1);
        }

        void *BitStream::Read(Kiaro::Common::U32 out_data_length, bool should_memcpy)
        {
            if (out_data_length > this->current_data_index)
                this->current_data_index = 0;
            else
                this->current_data_index -= out_data_length;

            if (should_memcpy)
            {
                //void *memory = new Common::u8[out_data_length];
                void *memory = (void*)new Kiaro::Common::U8[out_data_length];
                memcpy(memory, &this->current_data_array[this->current_data_index], out_data_length);
                return memory;
            }

            return (void*)&this->current_data_array[this->current_data_index];
        }

        Kiaro::Common::F32 BitStream::ReadF32(bool should_memcpy)
        {
            return *(Kiaro::Common::F32*)Read(sizeof(Kiaro::Common::F32), should_memcpy);
        }

        bool BitStream::ReadBool(bool should_memcpy)
        {
            return *(bool*)Read(sizeof(bool), should_memcpy);
        }

        Kiaro::Common::U8 BitStream::ReadU8(bool should_memcpy)
        {
            return *(Kiaro::Common::U8*)Read(sizeof(Kiaro::Common::U8), should_memcpy);
        }

        Kiaro::Common::U32 BitStream::ReadU32(bool should_memcpy)
        {
            return *(Kiaro::Common::U32*)Read(sizeof(Kiaro::Common::U32), should_memcpy);
        }

        Kiaro::Common::C8 *BitStream::ReadString(Kiaro::Common::U32 out_string_length, bool should_memcpy)
        {
        	// If the Length if zero, try to figure out where we should stop, then
        	// FIXME (Robert MacGregor#9): Go out until maximum arb string length
        	for (Kiaro::Common::U32 i = this->current_data_index; i < this->current_data_length; i++)
				if (this->current_data_array[i] == 0x00)
				{
					out_string_length = i - this->current_data_length;
					break;
				}

            return (Kiaro::Common::C8*)Read(out_string_length, should_memcpy);
        }

        void *BitStream::Raw(bool should_memcpy)
        {
            return Read(this->current_data_index, should_memcpy);
        }

        Kiaro::Common::U32 BitStream::TotalSize(void)
        {
            return this->current_data_length;
        }
    } // End Namespace Support
} // End namespace Kiaro
