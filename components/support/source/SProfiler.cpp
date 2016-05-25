/**
 *  @file SProfiler.cpp
 *  @brief Include file defining the Kiaro::Support::CBitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/SProfiler.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SProfiler* sInstance = nullptr;

        SProfiler* SProfiler::getPointer(const size_t sampleCount)
        {
            if (!sInstance)
                sInstance = new SProfiler(sampleCount);

            return sInstance;
        }

        void SProfiler::destroy(void)
        {
            delete sInstance;
            sInstance = nullptr;
        }

        SProfiler::SProfiler(const size_t sampleCount) : mSample(0), mSampleCount(sampleCount)
        {
            // Populate the set
            Support::UnorderedMap<Support::String, Common::F32> inserted;
            for (size_t iteration = 0; iteration < mSampleCount; ++iteration)
                mSamples.insert(mSamples.end(), inserted);
        }

        SProfiler::~SProfiler(void)
        {

        }

        void SProfiler::scopeBegin(const Support::String& name)
        {
            mSamples[mSample][name] = 0;
            mSampleNames.insert(name);

            // If there's already a timer, we're in a bad situation
            if (mTimers.find(name) != mTimers.end())
                throw std::runtime_error("Broken profiler scope!");

            mTimers[name] = FTime::startTimer();
        }

        void SProfiler::scopeEnd(const Support::String& name)
        {
            auto it = mSamples[mSample].find(name);

            if (it != mSamples[mSample].end() && mTimers.find(name) != mTimers.end())
            {
                mSamples[mSample][name] = FTime::stopTimer(mTimers[name]);
                mTimers.erase(name);
            }
            else // If we don't find anything here, we've got broken code somewhere
                throw std::runtime_error("No such profiler context!");
        }

        const Common::F32& SProfiler::getSample(const Support::String& name, const size_t sample)
        {
            if (sample >= mSampleCount || mSamples[sample].find(name) == mSamples[sample].end())
                throw std::out_of_range("No such sample!");

            return mSamples[sample][name];
        }

        Common::F32 SProfiler::getAverage(const Support::String& name)
        {
            if(mSampleNames.find(name) == mSampleNames.end())
                throw std::out_of_range("No such sample!");

            Common::F32 sum = 0;
            for (size_t iteration = 0; iteration < mSampleCount; iteration++)
                sum += mSamples[iteration][name];

            return sum / mSampleCount;
        }

        const Support::UnorderedSet<Support::String>& SProfiler::getSampleNames(void)
        {
            return mSampleNames;
        }

        Support::Set<std::pair<Support::String, Common::F32>> SProfiler::getSampleAverages(void)
        {
            Support::Set<std::pair<Support::String, Common::F32>> result;

            for (const std::string& name: mSampleNames)
                result.insert(std::make_pair(name, this->getAverage(name)));

            return result;
        }

        void SProfiler::update(void)
        {
            ++mSample %= mSampleCount;
        }
    } // End NameSpace Support
} // End nameSpace Kiaro


