/**
 *  @file SProfiler.hpp
 *  @brief Include file defining the Kiaro::Support::CBitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_SPROFILER_H_
#define _INCLUDE_SUPPORT_SPROFILER_H_

#define PROFILER_BEGIN(name) Support::SProfiler::getPointer()->scopeBegin(#name)
#define PROFILER_END(name) Support::SProfiler::getPointer()->scopeEnd(#name)

#include <support/common.hpp>
#include <support/String.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedMap.hpp>
#include <support/UnorderedSet.hpp>
#include <support/FTime.hpp>

namespace Kiaro
{
    namespace Support
    {
        class SProfiler
        {
            // Private Members
            private:
                size_t mSample;
                Support::Vector<Support::UnorderedMap<Support::String, Common::F32>> mSamples;
                Support::UnorderedMap<Support::String, FTime::timer> mTimers;
                Support::UnorderedSet<Support::String> mSampleNames;

            // PUblic Members
            public:
                const size_t mSampleCount;

            // Public Methods
            public:
                /**
                 *  @brief Returns an instance to the profiler singleton.
                 *  @return A pointer to the currently allocated SProfiler. If
                 *  this is the first call, then a new SProfiler will be allocated.
                 */
                static SProfiler* getPointer(const size_t& sampleCount = 32);

                /**
                 *  @brief Destroys the existing profiler singleton.
                 */
                static void destroy(void);

                void scopeBegin(const Support::String& name);
                void scopeEnd(const Support::String& name);

                void update(void);

                const Common::F32& getSample(const Support::String& name, const size_t& sample);

                Common::F32 getAverage(const Support::String& name);

                const Support::UnorderedSet<Support::String>& getSampleNames(void);

                Support::Set<std::pair<Support::String, Common::F32>> getSampleAverages(void);

            // Private Methods
            private:
                SProfiler(const size_t& sampleCount = 32);
                ~SProfiler(void);
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_SUPPORT_SPROFILER_H_

