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
        /**
         *  @brief A built in profiler that monitors application resource usage through scoped
         *  timers placed throughout the running application.
         */
        class SProfiler
        {
            // Private Members
            private:
                //! The current sample number we're on.
                size_t mSample;

                //! A vector of all the sample data.
                Support::Vector<Support::UnorderedMap<Support::String, Common::F32>> mSamples;

                //! A mapping of sample names to their associated internal timers.
                Support::UnorderedMap<Support::String, FTime::timer> mTimers;

                //! A set of all registered sample names.
                Support::UnorderedSet<Support::String> mSampleNames;

            // PUblic Members
            public:
                //! Total number of samples we're operating with.
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

                /**
                 *  @brief Begins a profiler scope with the given name. There must be an associated
                 *  scopeEnd later in the application.
                 *  @param name The name of the resource we are monitoring.
                 *  @throw std::runtime_error Thrown when there is already a timer running for the
                 *  given resource.
                 */
                void scopeBegin(const Support::String& name);

                /**
                 *  @brief Ends a profiler scope with the given name. There must be an associated
                 *  scopeBegin earlier in the application.
                 *  @param name The name of the resource we are monitoring.
                 */
                void scopeEnd(const Support::String& name);

                void update(void);

                /**
                 *  @brief Returns the sample value for the given resource.
                 *  @param name The resource name to query.
                 *  @param sample The sample number to grab.
                 *  @throw std::out_of_range Thrown when either the resource or sample number
                 *  are not found.
                 */
                const Common::F32& getSample(const Support::String& name, const size_t& sample);

                /**
                 *  @brief Returns the average of the given resource across all currently recorded
                 *  samples.
                 *  @param name The resource name to calculate the average of.
                 *  @throw std::out_of_range Thrown when the resource by the given name cannot
                 *  be found.
                 */
                Common::F32 getAverage(const Support::String& name);

                /**
                 *  @brief Returns a set of all the resource names currently known to the profiler.
                 *  @return A set of all resource names currently known to the profiler.
                 */
                const Support::UnorderedSet<Support::String>& getSampleNames(void);

                /**
                 *  @brief Returns a set of all resources and their current average values.
                 *  @return A set of all resources and their current average values.
                 */
                Support::Set<std::pair<Support::String, Common::F32>> getSampleAverages(void);

            // Private Methods
            private:
                SProfiler(const size_t& sampleCount = 32);
                ~SProfiler(void);
        };
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_SUPPORT_SPROFILER_H_

