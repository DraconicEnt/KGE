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

#include <support/CBitStream.hpp>
#include <support/CCommandLineParser.hpp>
#include <support/CFileBitStream.hpp>
#include <support/CMapDivision.hpp>
#include <support/FEndian.hpp>
#include <support/FTime.hpp>

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

namespace Kiaro
{
    namespace Support
    {
        template <typename storedType>
        using Set = std::set<storedType>;

        template <typename storedType>
        using UnorderedSet = std::unordered_set<storedType>;

        template <typename keyType, typename storedType>
        using Map = std::map<keyType, storedType>;

        template <typename keyType, typename storedType>
        using UnorderedMap = std::unordered_map<keyType, storedType>;

        template <typename... storedTypes>
        using Tuple = std::tuple<storedTypes...>;

        template <typename storedType>
        using Atomic = std::atomic<storedType>;

        template <typename storedType>
        using Deque = std::deque<storedType>;

        template <typename storedType>
        using Vector = std::vector<storedType>;

        // TODO (Robert MacGregor#9): Compare template param
        template <typename storedType, typename containerType = Deque<storedType>>
        using PriorityQueue = std::priority_queue<storedType, containerType>;

        template <typename storedType, typename containerType = Deque<storedType>>
        using Queue = std::queue<storedType, containerType>;

        template <typename storedTypeOne, typename storedTypeTwo>
        using Pair = std::pair<storedTypeOne, storedTypeTwo>;

        typedef std::mutex Mutex;
        typedef std::thread Thread;
        typedef std::string String;
        typedef std::wstring WString;
    }
}
