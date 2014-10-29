/**
 *  @file String.cpp
 *  @brief Source code file defining the logic for the Kiaro::Support::String class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @todo Remove this completely.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <stdarg.h>

#include <engine/Config.hpp>

#include <boost/algorithm/string.hpp>

#include <support/String.hpp>

namespace Kiaro
{
    namespace Support
    {
        String::String(void) : String("") { }

        String::String(const Kiaro::Common::C8 *value, bool untrusted) : is_const(true), stored(0x00), length(0)
        {
            if (!value)
                return;

            if (!untrusted)
                this->length = strlen(value);
            else
                for (Kiaro::Common::U32 i = 0; i < MAXIMUM_ARBITRARY_STRING_LENGTH; i++)
                    if (value[i] == 0x00)
                    {
                        this->length = strlen(value);
                        break;
                    }

            this->stored = new Kiaro::Common::C8[this->length + 1];
            memcpy(this->stored, value, this->length + 1);
        }

        String::~String(void)
        {
            if (this->stored != 0x00 && !this->is_const)
				delete this->stored;
		}

        Kiaro::Common::U32 String::TotalLength(void) const
        {
            return this->length;
        }

        Kiaro::Common::C8 *String::C_Str(void) const
        {
            return this->stored;
        }

        String String::Concat(const String &other) const
        {
            if (this->stored == 0x00)
                return String(other.stored);
            else if (other.stored == 0x00)
                return String(this->stored);

			Kiaro::Common::U32 memory_size = length + other.length + 1;
            Kiaro::Common::C8 *result = new Kiaro::Common::C8[memory_size];

           // #if defined(_WINDOWS)
          //  sprintf_s(result, memory_size, "%s%s", stored, other.stored);
           // #else
            sprintf(result, "%s%s", stored, other.stored);
           // #endif
            return String(result);
        }

        Kiaro::Common::S32 String::Compare(const String &other) const
        {
            // TODO: Make sure this is correct functionality.
            if (this->stored == 0x00 || other.stored == 0x00)
                return 1;

            return strcmp(this->stored, other.stored);
        }

        String String::Sub(Kiaro::Common::U32 start, Kiaro::Common::U32 offset)
        {
            Kiaro::Common::C8 *result = new Kiaro::Common::C8[offset];
            if (start + offset > length)
                offset = length - offset;

            memcpy(result, &stored[start], offset);
            result[offset] = 0x00;

            return String(result);
        }

        String String::Format(String string_template, Kiaro::Common::U32 in_count, ...)
        {
            Kiaro::Common::C8 *result_string = new Kiaro::Common::C8[256];

            va_list argp;
            va_start(argp, in_count);
            vsnprintf(result_string, 256, string_template.C_Str(), argp);
            va_end(argp);

			return String(result_string);
        }

        Kiaro::Common::U32 String::StripChar(Kiaro::Common::U8 character)
        {
            Kiaro::Common::U32 total_removed = 0;

            for (Kiaro::Common::U32 i = 0; i < this->length; i++)
                if (this->stored[i] == character)
                {
                    // Push everything back
                    for (Kiaro::Common::U32 j = i; j < this->length; j++)
                        this->stored[j] = this->stored[j + 1];

                    this->length--;
                    total_removed++;

                    // TODO: Eventually allow for reallocating the string?
                }

            return total_removed;
        }

        bool String::IsNull(void)
        {
        	return this->stored == 0x00;
        }

        bool String::operator ==(const String &other) const
        {
            if (this->stored == 0x00 || other.stored == 0x00)
                return false;

			return boost::algorithm::equals(this->stored, other.stored);
        }

        bool String::operator !=(const String &other) const
        {
            return !this->operator ==(other);
        }

        String String::operator +(const String &other) const
        {
            return this->Concat(other);
        }

        void String::operator +=(const String &other)
        {
            if (this->stored == 0x00)
                return;
            else if (other.stored == 0x00)
                return;

			Kiaro::Common::U32 memory_size = this->length + other.length + 2;
            Kiaro::Common::C8 *result = new Kiaro::Common::C8[memory_size];

          //  #if defined(_WINDOWS)
         //   sprintf_s(result, memory_size, "%s%s", stored, other.stored);
          //  #else
            sprintf(result, "%s%s", this->stored, other.stored);
          //  #endif

            if (this->stored != 0x00 && !this->is_const)
                delete this->stored;

            this->stored = result;
        }
    } // End Namespace Support
} // End Namespace Kiaro
