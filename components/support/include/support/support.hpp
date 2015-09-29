/**
 *  @file support.cpp
 *  @brief Include file including all the engine support implementations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2015 Draconic Entertainment
 */

#include <irrlicht.h>

#include <support/CBitStream.hpp>
#include <support/CCommandLineParser.hpp>
#include <support/CFileBitStream.hpp>
#include <support/CMapDivision.hpp>
#include <support/FEndian.hpp>
#include <support/FTime.hpp>

#include <support/Regex.hpp>

#include <atomic>
#include <iostream>
#include <set>
#include <unordered_set>
#include <map>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <queue>
#include <deque>
#include <string>
#include <stack>

#include <support/Math.hpp>
#include <support/Tuple.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>
#include <support/Map.hpp>

namespace Kiaro
{
    namespace Support
    {
		//! A typedef to an std::atomic.
        template <typename storedType>
        using Atomic = std::atomic<storedType>;

		//! A typedef to an std::deque.
        template <typename storedType>
        using Deque = std::deque<storedType>;

        // TODO (Robert MacGregor#9): Compare template param
        template <typename storedType, typename containerType = Deque<storedType>>
        using PriorityQueue = std::priority_queue<storedType, containerType>;

		//! A typrdef to an std::queue.
        template <typename storedType, typename containerType = Deque<storedType>>
        using Queue = std::queue<storedType, containerType>;

		//! A typedef to an std::pair.
        template <typename storedTypeOne, typename storedTypeTwo>
        using Pair = std::pair<storedTypeOne, storedTypeTwo>;

		//! A typedef to an std::string.
		template <typename storedType>
		using Stack = std::stack<storedType>;

		//! A typedef to an std::mutex.
        typedef std::mutex Mutex;
		//! A typedef to an std::thread.
        typedef std::thread Thread;
		//! A typedef to an std::string.
        typedef std::string String;
		//! A typedef to an std::wstring.
        typedef std::wstring WString;

        //! A typedef to a 2D vector type that represents dimensions.
        template <typename storedType>
        using Dimension2D = irr::core::dimension2d<storedType>;

        //! A typedef to a 2D vector that represents dimensions using an unsigned int.
        typedef Dimension2D<Common::U32> Dimension2DU;
    }
}
