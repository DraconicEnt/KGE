/**
 *  @file ISingleton.hpp
 *  @brief Include file declaring the ISingleton singleton interface class type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_ISERIALIZABLE_HPP_
#define _INCLUDE_SUPPORT_ISERIALIZABLE_HPP_

#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;

        /**
         *  @brief An interface class representing a non-primitive type that may be packed and unpacked from an arbitrary
         *  bit stream.
         */
        class ISerializable
        {
            public:
                /**
                 *  @brief Packs all data about this ISerializable into the CBitStream. This is usually required for
                 *  new instantiations of objects.
                 *  @param out The CBitStream to write into.
                 */
                virtual void packEverything(Support::CBitStream& out) const = 0;

                /**
                 *  @brief Unpacks a payload from the current position in the input bit stream for this
                 *  ISerializable object.
                 *  @param in The input CBitStream to unpack from.
                 */
                virtual void unpack(Support::CBitStream& in) = 0;

                /**
                 *  @brief Returns the total number of bytes required to pack this ISerializable object
                 *  into a CBitStream.
                 *  @return The number of bytes required to have left in a CBitStream to be able to pack
                 *  this ISerializable.
                 */
                virtual size_t getRequiredMemory(void) const = 0;
        };
    }
}
#endif // _INCLUDE_SUPPORT_ISINGLETON_HPP_
