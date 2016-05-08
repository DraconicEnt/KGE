/**
 *  @file types.hpp
 *  @brief Include file including all the engine support implementations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_TYPES_HPP_
#define _INCLUDE_SUPPORT_TYPES_HPP_

#include <irrlicht.h>

#include <atomic>
#include <iostream>
#include <unordered_set>
#include <map>
#include <mutex>
#include <thread>
#include <tuple>
#include <utility>
#include <queue>

#include <support/Stack.hpp>
#include <support/common.hpp>
#include <support/Deque.hpp>
#include <support/UnorderedMap.hpp>
#include <support/UnorderedSet.hpp>
#include <support/Vector.hpp>
#include <support/String.hpp>
#include <support/Set.hpp>

namespace Kiaro
{
    namespace Support
    {
		//! A typedef to an std::atomic.
        template <typename storedType>
        using Atomic = std::atomic<storedType>;

        // TODO (Robert MacGregor#9): Compare template param
        template <typename storedType, typename containerType = Deque<storedType>>
        using PriorityQueue = std::priority_queue<storedType, containerType>;

		//! A typrdef to an std::queue.
        template <typename storedType, typename containerType = Deque<storedType>>
        using Queue = std::queue<storedType, containerType>;

		//! A typedef to an std::pair.
        template <typename storedTypeOne, typename storedTypeTwo>
        using Pair = std::pair<storedTypeOne, storedTypeTwo>;

		//! A typedef to an std::mutex.
        typedef std::mutex Mutex;
		//! A typedef to an std::thread.
        typedef std::thread Thread;
		//! A typedef to an std::wstring.
        typedef std::wstring WString;

        //! A typedef to a 2D vector type that represents dimensions.
        template <typename storedType>
        using Dimension2D = irr::core::dimension2d<storedType>;

        //! A typedef to a 2D vector that represents dimensions using an unsigned int.
        typedef Dimension2D<Common::U32> Dimension2DU;
    }
}
#endif // _INCLUDE_SUPPORT_TYPES_HPP_
