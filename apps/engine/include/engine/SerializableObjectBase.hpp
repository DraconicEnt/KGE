/**
 *  @file SerializableObjectBase.hpp
 *  @brief Include file defining the SerializableObjectBase class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_
#define _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_
namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Engine
    {
        /**
         *  @brief A base class representing an object that can be serialized to
         *  and from a Kiaro::Support::BitStream.
         */
       class SerializableObjectBase
       {
            // Public Methods
            public:
                virtual void packData(Kiaro::Support::BitStream &out) = 0;
                virtual void unpackData(Kiaro::Support::BitStream &in) = 0;
       };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_
