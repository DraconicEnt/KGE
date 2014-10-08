/**
 *  @file String.h
 *  @brief Include file that defines the Kiaro::Support::String class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_STRING_H_
#define _INCLUDE_KIARO_SUPPORT_STRING_H_

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        //! Inbuilt String class for the Kiaro Game Engine.
        class String
        {
            public:
            //! Standard Constructor.
            String(void);

            /**
             *  @brief Constructor accepting a c8 array.
             *  @param value A c8 array that is our input string.
             */
            String(const Kiaro::Common::C8 *value, bool untrusted = false);

            //! Standard destructor.
            ~String(void);

            /**
             *  @brief Returns the length of the String in characters.
             *  @return A u32 representing the length of the String in characters.
             */
            Kiaro::Common::U32 TotalLength(void) const;

            /**
             *  @brief Returns the internal C string representation of the String.
             *  @return The c8 array that is internally stored for this String.
             */
            Kiaro::Common::C8 *C_Str(void) const;

            /**
             *  @brief Concatenates the two strings.
             *  @param other A String to combine with.
             *  @return A new String representing the concatenated strings.
             */
            String Concat(const String &other) const;
            Common::S32 Compare(const String &other) const;

            /**
             *  @brief Returns the substring of a string.
             *  @note If offset exceeds the bounds of the string, then the entire portion
             *  of the string from where start identifies to the very end is returned.
             */
            String Sub(Common::U32 start, Kiaro::Common::U32 offset);
            static String Format(String string_template, Kiaro::Common::U32 in_count, ...);

            Kiaro::Common::U32 StripChar(Kiaro::Common::U8 character);

            bool IsNull(void);

            //! Operator overload that checks if the strings are equivalent.
            bool operator ==(const String &other) const;
            //! Operator overload that checks if the strings are not equivalent.
            bool operator !=(const String &other) const;
            //! Operator overload for the String.concat operation.
            String operator +(const String &other) const;
            //! Operator overload for String.concat -and- write to the strng.
            void operator +=(const String &other);

            private:
            // TODO (Robert MacGregor#9): Figure out why is_const is necessary for the strings to operate correctly and if it has issues elsewhere.
            bool is_const;

            Common::C8 *stored;
            Common::U32 length;
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_STRING_H_
