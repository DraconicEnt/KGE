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


        class Vector3DF 
        {
            public:
                float x;
                float y;
                float z;

                Vector3DF(float x, float y, float z)
                {
                    this->x = x;
                    this->y = y;
                    this->z = z;
                }
                
                Vector3DF(const Vector3DF& rhs)
                {
                    this->x = rhs.x;
                    this->y = rhs.y;
                    this->z = rhs.z;
                }

                Vector3DF(void)
                {
                    this->x = this->y = this->z = 0;
                }
        };

        class Vector2DF 
        {

        };

        class Dimension2DU 
        {
            public:
                unsigned int x;
                unsigned int y;

                Dimension2DU(unsigned int x, unsigned int y)
                {
                    this->x = x;
                    this->y = y;
                }

                Dimension2DU(const Dimension2DU& rhs)
                {
                    this->x = rhs.x;
                    this->y = rhs.y;
                }

                Dimension2DU(void)
                {
                    this->x = this->y = 0;
                }
        };

        class ColorRGBA 
        {
            public:
                unsigned int r;
                unsigned int g;
                unsigned int b;
                unsigned int a;

                ColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
                {
                    this->r = r;
                    this->g = g;
                    this->b = b;
                    this->a = a;
                }
        };
    }
}
#endif // _INCLUDE_SUPPORT_TYPES_HPP_