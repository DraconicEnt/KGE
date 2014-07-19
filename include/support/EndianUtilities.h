/**
 *  @file EndianUtilities.cpp
 *  @brief Source code file defining the logic for the Kiaro::Support::String class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <EngineCommon.h>

namespace Kiaro
{
    namespace Support
    {
        static inline void SwapEndianU32Ref(Kiaro::Common::U32 &from);
        static inline Kiaro::Common::U32 SwapEndianU32(const Kiaro::Common::U32 &from);

        static inline void SwapEndianU64Ref(Kiaro::Common::U64 &from);
        static inline Kiaro::Common::U64 SwapEndianU64(const Kiaro::Common::U64 &from);

        static inline void SwapEndianU16Ref(Kiaro::Common::U16 &from);
        static inline Kiaro::Common::U16 SwapEndianU16(const Kiaro::Common::U16 &from);

        static inline void SwapEndianF32Ref(Kiaro::Common::F32 &from);
        static inline Kiaro::Common::F32 SwapEndianF32(const Kiaro::Common::F32 &from);

        static inline void SwapEndianArbitrary(void *from, const Kiaro::Common::U32 &size);
    }
}
