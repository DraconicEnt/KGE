/**
 *  @file easydelegate.hpp
 *  @version 1.1
 *  @date 9/25/2014
 *  @author <a href="https://github.com/Ragora">Robert MacGregor</a>
 *  @brief Portable delegate system that should work on any C++11 compliant compiler.
 *  @details EasyDelegate is a library that exposes an easy and flexible delegate system with
 *  the use of C++11's variatic templates.
 *  @todo Mapping of various delegate for the DelegateSet type values like the this pointer, proc address
 *  and the delegate instance pointer to helpful data that will provide non O(N) access when using methods
 *  such as remove_delegate. This would also allow for a system in which the end programmer can quickly
 *  grab a list of delegates that meet a certain condition such as a specific this pointer or proc address.
 *  An std::map is going to be utilized for these implementations.
 *  @todo Associated DelegateSet storing so that if a delegate instance is
 *  deleted directly it can be automatically removed from any DelegateSet
 *  instances it may be inside of. This will be done an std::vector
 *  that will be iterated over.
 *
 *  The use of the size_t type throughout the internal implementation is designed for x86 systems
 *  where sizeof(size_t) is equivalent to sizeof(largest integer type), which has been true for GCC
 *  and MSVS on Linux and Windows systems.
 *
 *  @copyright This software is licensed under the Lesser General Public License version 3.
 *  Please refer to GPL.txt and LGPL.txt for more information.
 *  @example ex_delegateset.cpp
 */

#include <stdexcept>    // Standard exception types
#include <assert.h>     // assert(expr)
#include <vector>       // std::vector<type>
#include <tuple>        // std::tuple<...>

#ifndef _INCLUDE_EASYDELEGATE_HPP_
#define _INCLUDE_EASYDELEGATE_HPP_

//! Namespace containing all EasyDelegate functionality.
namespace EasyDelegate
{
    /**
     *  @brief A type that can represent any delegate type, but it cannot be invoked
     *  without casting to a delegate type that knows the proper function signature.
     *  @details This delegate type is useful in the event you wish to pass
     *  around delegates regardless of the type in a system where the function
     *  signature is resolved via other means for type casting and invocation.
     */
    class GenericDelegate
    {
        public:
            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate
             *  is an instance of MemberDelegate.
             */
            GenericDelegate(const bool &isMemberDelegate, const bool &isCachedDelegate) : mIsMemberDelegate(isMemberDelegate),
            mIsCachedDelegate(isCachedDelegate) { }

            //! A boolean representing whether or not this delegate is a member delegate.
            const bool mIsMemberDelegate;
            //! A boolean representing whether or not this delegate is a cached delegate.
            const bool mIsCachedDelegate;
    };

    /**
     *  @brief Forward declaration for DelegateBase.
     *  @details This is done because the real DelegateBase declaration
     *  at the bottom of this file utilizes typedefs against the StaticDelegate
     *  and MemberDelegate types but at the same time, MemberDelegate and StaticDelegate
     *  derive from DelegateBase to allow storage of both types in the DelegateSet type.
     */
    template <typename returnType, typename... parameters>
    class DelegateBase;

    /**
     *  @brief A delegate of a static function.
     *  @details The function that is to be made into a delegate must have
     *  it's return type and argument information specified in the template.
     */
    template <typename returnType, typename... parameters>
    class StaticDelegate : public DelegateBase<returnType, parameters...>
    {
        public:
            //! Helper typedef referring to a static function pointer that is compatible with this delegate.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

            /**
             *  @brief Constructor accepting a static function.
             *  @param procAddress The static function that this delegate
             *  is intended to invoke.
             */
            StaticDelegate(StaticDelegateFuncPtr procAddress) : DelegateBase<returnType, parameters...>(false), mProcAddress(procAddress) { }

            /**
             *  @brief Standard copy constructor.
             *  @param other An instance of a StaticDelegate with the same function signature to copy.
             */
            StaticDelegate(const StaticDelegate<returnType, parameters...>& other) : DelegateBase<returnType, parameters...>(false),
            mProcAddress(other.mProcAddress) { }

            /**
             *  @brief Invoke the StaticDelegate.
             *  @param params Anything; It depends on the function signature specified in the template.
             *  @return Anything; It depends on the function signature specified in the template.
             *  @throw std::runtime_error Thrown when the static function that this
             *  delegate is supposed to be invoking is NULL.
             *  @throw std::exception Potentially thrown by the function called by
             *  this StaticDelegate.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            returnType invoke(parameters... params)
            {
                assert(this->mProcAddress);

                if (!this->mProcAddress)
                    throw std::runtime_error("Bad StaticDelegate Proc Address");

                return ((StaticDelegateFuncPtr)this->mProcAddress)(params...);
            }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this StaticDelegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false because StaticDelegates don't use a this pointer.
             */
            bool has_thispointer(void *thisPointer) { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the given static proc address.
             *  @param procAddress A pointer to the static function to be checked against.
             *  @return A boolean representing whether or not this StaticDelegate calls the given proc address.
             */
            bool has_procaddress(StaticDelegate::StaticDelegateFuncPtr procAddress) { return mProcAddress == procAddress; }

        private:
            //! Internal pointer to proc address to be called.
            StaticDelegateFuncPtr mProcAddress;
    };

    /**
     *  @brief A delegate of a class member function.
     *  @details The function that is to be made into a delegate must have
     *  the class type and it's return type and argument information specified in the template.
     */
    template <typename classType, typename returnType, typename... parameters>
    class MemberDelegate : public DelegateBase<returnType, parameters...>
    {
        public:
            //! Helper typedef referring to a member function pointer.
            typedef returnType(classType::*MemberDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);

            /**
             *  @brief Constructor accepting a this pointer and a member function.
             *  @param thisPtr A pointer to the object instance to be considered 'this'
             *  during invocation.
             *  @param procAddress A pointer to the member function to be invoked upon
             *  'this'.
             *  @note The MemberDelegate invocation code has no way of knowing if the 'this'
             *  pointer at any time has been deallocated. The MemberDelegate will cause undefined
             *  behavior and/or segfault upon invocation in that case.
             */
            MemberDelegate(classType *thisPtr, MemberDelegateFuncPtr procAddress) : mThisPtr(thisPtr),
            mProcAddress(procAddress), DelegateBase<returnType, parameters...>(true) { }

            /**
             *  @brief Standard copy constructor.
             */
            MemberDelegate(const MemberDelegate<classType, returnType, parameters...>& other) : mThisPtr(other.mThisPtr),
            mProcAddress(other.mProcAddress), DelegateBase<returnType, parameters...>(true) { }

            /**
             *  @brief Invoke the MemberDelegate.
             *  @param params Anything; It depends on the function signature specified in the template.
             *  @return Anything; It depends on the function signature specified in the template.
             *  @throw std::runtime_error Thrown when the static function that this
             *  delegate is supposed to be invoking is NULL.
             *  @throw std::runtime_error Thrown when the MemberDelegate's this pointer
             *  is NULL.
             *  @throw std::exception Potentially thrown by the function called by
             *  this MemberDelegate.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            returnType invoke(parameters... params)
            {
                assert(this->mThisPtr);
                assert(this->mProcAddress);

                if (!this->mThisPtr)
                    throw std::runtime_error("Invalid MemberDelegate This Pointer");

                if (!this->mProcAddress)
                    throw std::runtime_error("Invalid MemberDelegate Proc Address");

                classType *thisPtr = (classType*)this->mThisPtr;
                return (thisPtr->*mProcAddress)(params...);
            }

            /**
             *  @brief Returns whether or not this MemberDelegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this MemberDelegate calls a member function
             *  against the given this pointer.
             */
            bool has_thispointer(void *thisPointer) { return mThisPtr == thisPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the given member proc address.
             *  @param procAddress A pointer to a member function to be checked against.
             *  @return A boolean representing whether or not this MemberDelegate calls the given member proc address.
             */
            bool has_procaddress(MemberDelegate::MemberDelegateFuncPtr procAddress) { return mProcAddress == procAddress; }

            /**
             *  @brief Returns whether or not this MemeberDelegate calls the given static proc address.
             *  @param procAddress A pointer to the static function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false because a static function cannot be invoked by a MemberDelegate.
             */
            bool has_procaddress(MemberDelegate::StaticDelegateFuncPtr funcPtr) { return false; }

        private:
            //! An internal pointer to this object.
            const classType *mThisPtr;
            //! An internal pointer to the proc address to be called.
            const MemberDelegateFuncPtr mProcAddress;
    };

    /**
     *  @brief A set of delegates with manipulation similar to std::vector.
     *  @note Both MemberDelegate and StaticDelegate instances may be used with
     *  this.
     *  @note The contained delegate instances are deleted upon deletion.
     */
    template <typename returnType, typename... parameters>
    class DelegateSet
    {
        private:
            //! Helper typedef to construct the function pointer signature from the template.
            typedef returnType(*delegateFuncPtr)(parameters...);
            //! Helper typedef to construct the StaticDelegate signature from the template.
            typedef DelegateBase<returnType, parameters...> DelegateBaseType;

        public:
            //! Helper typedef for when building static delegates for this set.
            typedef StaticDelegate<returnType, parameters...> StaticDelegateType;
            //! Helper typedef for when building member delegates for this set.
            template <typename classType>
            using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;
            //! Helper typedef for when wanting the return type of this set.
            typedef returnType ReturnType;

            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

            //! Standard constructor.
            DelegateSet(void) { }

            //! Standard destructor.
            ~DelegateSet(void)
            {
                for (typename std::vector<DelegateSet::DelegateBaseType *>::iterator it = mDelegateVector.begin(); it != mDelegateVector.end(); it++)
                    delete (*it);
            }

            /**
             *  @brief Pushes a delegate instance to the end of the set.
             *  @param delegateInstance The delegate instance to the pushed onto the set.
             */
            void push_back(DelegateSet::DelegateBaseType *delegateInstance) { mDelegateVector.push_back(delegateInstance); }

            //! Alternate to push_back that looks a bit prettier in source.
            void operator +=(DelegateSet::DelegateBaseType *delegateInstance) { mDelegateVector.push_back(delegateInstance); }

            /**
             *  @brief Inserts a given function as a delegate at it.
             *  @param it The iterator representing the position to insert at.
             *  @param in The static function to insert as a delegate.
             */
            void insert(typename std::vector<DelegateSet::DelegateBaseType *>::iterator it, delegateFuncPtr in) { mDelegateVector.insert(it, new DelegateSet::DelegateBaseType(in)); }

            /**
             *  @brief Inserts a given StaticDelegate instance at it.
             *  @param it The iterator representing the position to insert at.
             *  @param in the StaticDelegate instance to insert.
             */
            void insert(typename std::vector<DelegateSet::DelegateBaseType *>::iterator it, DelegateSet::DelegateBaseType *in) { mDelegateVector.insert(it, in); }

            /**
             *  @brief Erase the given index or range of indices.
             *  @param startIndex A size_t representing the index to start at.
             *  @param endIndex A size_t whose default value is 0 representing a range
             *  of delegates between startIndex and endIndex, inclusive.
             */
            void erase(size_t startIndex, size_t endIndex=0) { mDelegateVector.erase(mDelegateVector.begin() + startIndex, mDelegateVector.begin() + startIndex + endIndex); }

            /**
             *  @brief Removes all delegates from the set that have the given static proc address
             *  for it's method.
             *  @param procAddress The static proc address to check against.
             *  @param deleteInstances A boolean representing whether or not all matches should be deleted when removed. The default
             *  for this parameter is true for better memory management.
             *  @param out A pointer to an std::vector templated for the DelegateBase type that this DelegateSet contains. This vector
             *  will then be populated with a list of removed delegates. If deleteInstances is false, this list will never be populated.
             *  @warning If deleteInstances is false and there is no out specified, you will be leaking memory if there is no other
             *  delegate tracking mechanism implemented by your project.
             */
            template <typename className>
            void remove_delegate_procaddress(DelegateSet::MemberDelegateFuncPtr<className> procAddress, bool deleteInstances=true, std::vector<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                size_t currentDelegateCount = mDelegateVector.size();

                for (size_t iteration = 0; iteration < currentDelegateCount; iteration++)
                    if (mDelegateVector[iteration]->has_procaddress(procAddress))
                    {
                        if (deleteInstances)
                            delete mDelegateVector[iteration];
                        else if (out)
                            out->push_back(mDelegateVector[iteration]);

                        mDelegateVector.erase(mDelegateVector.begin() + iteration);
                        currentDelegateCount--;

                        // Subtract one from iteration so we don't skip the element that now resides at iteration
                        iteration--;
                    }
            }

            /**
             *  @brief Removes all delegates from the set that have the given static proc address
             *  for it's method.
             *  @param procAddress The static proc address to check against.
             *  @param deleteInstances A boolean representing whether or not all matches should be deleted when removed. The default
             *  for this parameter is true for better memory management.
             *  @param out A pointer to an std::vector templated for the DelegateBase type that this DelegateSet contains. This vector
             *  will then be populated with a list of removed delegates. If deleteInstances is false, this list will never be populated.
             *  @warning If deleteInstances is false and there is no out specified, you will be leaking memory if there is no other
             *  delegate tracking mechanism implemented by your project.
             */
            void remove_delegate_procaddress(DelegateSet::StaticDelegateFuncPtr procAddress, bool deleteInstances=true, std::vector<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                size_t currentDelegateCount = mDelegateVector.size();

                for (size_t iteration = 0; iteration < currentDelegateCount; iteration++)
                    if (mDelegateVector[iteration]->has_procaddress(procAddress))
                    {
                        if (deleteInstances)
                            delete mDelegateVector[iteration];
                        else if (out)
                            out->push_back(mDelegateVector[iteration]);

                        mDelegateVector.erase(mDelegateVector.begin() + iteration);

                        iteration--;
                        currentDelegateCount--;
                    }
            }

            /**
             *  @brief Removes a all MemberDelegate types from the set that have a given 'this' pointer address
             *  to call against.
             *  @param thisPtr The address of the object to check against.
             *  @param deleteInstances A boolean representing whether or not all matches should be deleted when removed. The default
             *  for this parameter is true for better memory management.
             *  @param out A pointer to an std::vector templated for the DelegateBase type that this DelegateSet contains. This vector
             *  will then be populated with a list of removed delegates. If deleteInstances is false, this list will never be populated.
             *  @warning If deleteInstances is false and there is no out specified, you will be leaking memory if there is no other
             *  delegate tracking mechanism implemented by your project.
             */
            void remove_delegate_this(void *thisPtr, const bool &deleteInstances=true, std::vector<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                size_t currentDelegateCount = mDelegateVector.size();

                for (size_t iteration = 0; iteration < currentDelegateCount; iteration++)
                    if (mDelegateVector[iteration]->mIsMemberDelegate && mDelegateVector[iteration]->has_thispointer(thisPtr))
                    {
                        if (deleteInstances)
                            delete mDelegateVector[iteration];
                        else if (out)
                            out->push_back(mDelegateVector[iteration]);

                        mDelegateVector.erase(mDelegateVector.begin() + iteration);

                        iteration--;
                        currentDelegateCount--;
                    }
            }

            /**
             *  @brief Removes a given delegate by it's address.
             *  @param instance The delegate to attempt to remove from this set.
             *  @param deleteInstance A boolean representing whether or not the target delegate should
             *  be deleted.
             *  @return A pointer to the delegate that was removed. This is NULL if none were removed or
             *  if deleteInstance is true.
             */
            DelegateSet::DelegateBaseType *remove_delegate(DelegateSet::DelegateBaseType *instance, bool deleteInstance=true)
            {
                for (size_t iteration = 0; iteration < mDelegateVector.size(); iteration++)
                    if (mDelegateVector[iteration] == instance)
                    {
                        DelegateSet::DelegateBaseType *currentDelegate = mDelegateVector[iteration];

                        if (deleteInstance)
                        {
                            delete currentDelegate;
                            currentDelegate = NULL;
                        }

                        mDelegateVector.erase(mDelegateVector.begin() + iteration);
                        return currentDelegate;
                    }

                return NULL;
            }

            /**
             *  @brief Invoke all delegates in the set, ignoring any return values.
             *  @param args All other arguments that will be used as parameters to each delegate.
             *  @throw std::runtime_error Thrown when assertions are disabled and either a stored MemberDelegate
             *  or StaticDelegate type have a NULL function to call.
             *  @throw std::runtime_error Thrown when assertions are disabled and a stored MemberDelegate is
             *  attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            void invoke(parameters... params) const
            {
                for (typename std::vector<DelegateSet::DelegateBaseType *>::const_iterator it = mDelegateVector.begin(); it != mDelegateVector.end(); it++)
                    (*it)->invoke(params...);
            }

            /**
             *  @brief Invoke all delegates in the set, storing return values in out.
             *  @param out The std::vector that all return values will be sequentially written to.
             *  @param args All other arguments that will be used as parameters to each delegate.
             *  @throw std::runtime_error Thrown when assertions are disabled and either a stored MemberDelegate
             *  or StaticDelegate type have a NULL function to call.
             *  @throw std::runtime_error Thrown when assertions are disabled and a stored MemberDelegate is
             *  attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            void invoke(std::vector<returnType> &out, parameters... params) const
            {
                for (typename std::vector<DelegateSet::DelegateBaseType *>::const_iterator it = mDelegateVector.begin(); it != mDelegateVector.end(); it++)
                    out.push_back((*it)->invoke(params...));
            }

            /**
             *  @brief Get an iterator to the beginning of the set.
             *  @return An std::iterator referencing the beginning of the set.
             */
            typename std::vector<DelegateSet::DelegateBaseType *>::iterator begin(void) { return mDelegateVector.begin(); }

            /**
             *  @brief Get an iterator to the end of the set.
             *  @return An std::const_iterator referencing the end of the set.
             */
            typename std::vector<DelegateSet::DelegateBaseType *>::const_iterator end(void) { return mDelegateVector.end(); }

            /**
             *  @brief Returns whether or not the DelegateSet is empty.
             *  @return A boolean representing whether or not the DelegateSet
             *  is empty.
             */
            bool empty(void) { return mDelegateVector.empty(); }

            /**
             *  @brief Returns the size of the DelegateSet.
             *  @return The size in elements.
             */
            size_t size(void) { return mDelegateVector.size(); }

            /**
             *  @brief Array access.
             *  @param index A size_t representing the index to look at.
             *  @return A delegate at the given index.
             */
            DelegateSet::DelegateBaseType *operator[](size_t index) { return mDelegateVector[index]; }

            //! Helper typedef to get an std::iterator that corresponds to this DelegateSet.
            typedef typename std::vector<DelegateSet::DelegateBaseType *>::iterator iterator;
            //! Helper typedef to get an std::const_iterator that corresponds to this DelegateSet.
            typedef typename std::vector<DelegateSet::DelegateBaseType *>::const_iterator const_iterator;

        private:
            //! Internal vector storing the StaticDelegate instances.
            std::vector<DelegateSet::DelegateBaseType *> mDelegateVector;
    };


    // Taken from the chosen answer of
    // http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
    template<int ...> struct seq {};
    template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};
    template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

    /**
     *  @brief A type whose purpose is to provide deferred calling capabilities, as the name applies.
     */
    class GenericCachedDelegate : public GenericDelegate
    {
        public:
            /**
             *  @brief Constructor accepting a GenericDelegate pointer.
             *  @param delegate A pointer to the GenericDelegate to be stored on the CachedDelegate.
             */
            GenericCachedDelegate(GenericDelegate *delegate) : GenericDelegate(delegate->mIsMemberDelegate, true)
            {

            }

            /**
             *  @brief Dispatches the GenericCachedDelegate, ignoring the return value.
             *  @details This behaves exactly as the dispatch method available on CachedDelegate
             *  types except it does not care about the return value of the invoked delegate. You
             *  have to cast to a CachedDelegate with the proper function signature in order to
             *  dispatch it and get a return value.
             */
            virtual void generic_dispatch(void) = 0;
    };

    /**
     *  @brief A delegate type with which you can perform deferred calls.
     *  @details This is essentially just a wrapper for either a StaticDelegate or MemberDelegate
     *  instance but it caches any arguments for later invocation.
     */
    template <typename returnType, typename... parameters>
    class CachedDelegate : public GenericCachedDelegate
    {
        // Public Methods
        public:
            /**
             *  @brief Constructor accepting a delegate instance and function call parameters.
             *  @param delegate A pointer to the delegate instance that this CachedDelegate is supposed to invoke.
             *  @param params Anything; these parameters depend on the function signature defined in the template.
             */
            CachedDelegate(DelegateBase<returnType, parameters...> *delegate, parameters... params) : GenericCachedDelegate(delegate),
            mDelegate(delegate),
            mParameters(params...)
            {

            }

            /**
             *  @brief Standard destructor.
             *  @note This deletes the stored delegate instance as well, so the deletion of
             *  any CachedDelegate types will invalidate the delegate it was calling.
             */
            ~CachedDelegate(void) { delete mDelegate; mDelegate = NULL; }

            /**
             *  @brief Dispatches the CachedDelegate.
             *  @details This is equivalent to the invoke() method on all other delegate
             *  types except the parameters were cached at creation. Said cached parameters
             *  will be passed in automatically upon calling this, so it is completely safe
             *  to store.
             *  @return Anything; it depends on the function signature defined in the template.
             */
            returnType dispatch(void)
            {
                return performCachedCall(typename gens<sizeof...(parameters)>::type());
            }

            /**
             *  @brief Dispatches the CachedDelegate, ignoring the return value.
             *  @details This behaves exactly as the dispatch method above except it does not
             *  care about the return of the called function. This method is also callable on
             *  the CachedDelegateBase type, unlike the normal dispatch method.
             */
            void generic_dispatch(void) { dispatch(); }

            /**
             *  @brief Gets the internally invoked delegate.
             *  @return A pointer to the internally invoked delegate.
             */
            const DelegateBase<returnType, parameters...> *get_delegate(void) { return mDelegate; }

        // Private Methods
        private:
            //! Internal templated method to invoke the stored delegate instance.
            template<int ...S>
            returnType performCachedCall(seq<S...>)
            {
                assert(mDelegate);

                if (!mDelegate)
                    throw std::runtime_error("Bad CachedDelegate mDelegate pointer!");

                return mDelegate->invoke(std::get<S>(mParameters) ...);
            }

        // Public Members
        public:
            //! Helper typedef to a StaticDelegate.
            typedef StaticDelegate<returnType, parameters...> StaticDelegateType;
            template <typename classType>
            //! Helper typedef to a MemberDelegate.
            using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;

        // Private Members
        private:
            //! Internal std::tuple that is utilized to cache the parameter list.
            const std::tuple<parameters...> mParameters;
            //! Pointer to the internally invoked delegate instance.
            DelegateBase<returnType, parameters...> *mDelegate;
    };

    /**
     *  @brief Base delegate type.
     *  @details Inheritance from this type allows for both the StaticDelegate
     *  and MemberDelegate instances to be used in the DelegateSet type as it
     *  specifies a common interface for the two types to be sharing.
     */
    template <typename returnType, typename... parameters>
    class DelegateBase : public GenericDelegate
    {
        public:
            //! Helper typedef referring to the return type of this delegate.
            typedef returnType ReturnType;
            //! Helper typedef for when building static delegates.
            typedef StaticDelegate<returnType, parameters...> StaticDelegateType;
            //! Helper typedef for when building member delegates.
            template <typename classType>
            using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;

            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate is a
             *  member delegate.
             */
            DelegateBase(const bool &isMemberDelegate) : GenericDelegate(isMemberDelegate, false) { }

            /**
             *  @brief Returns whether or not this delegate calls the given static proc address.
             *  @param procAddress A pointer to the static function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false for MemeberDelegate types because a MemberDelegate cannot invoke static
             *  functions.
             */
            virtual bool has_procaddress(DelegateBase::StaticDelegateFuncPtr procAddress) = 0;

            /**
             *  @brief Returns whether or not this delegate calls the given member proc address.
             *  @param procAddress A pointer to a member function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @details This is not declared as a wholly virtual function because a templated function
             *  cannot be virtual.
             *  @note Always returns false for StaticDelegate types because a StaticDelegate cannot
             *  call member functions.
             */
            template <typename className>
            bool has_procaddress(DelegateBase::MemberDelegateFuncPtr<className> procAddress) noexcept
            {
                // Don't try to check as a MemberDelegate if we're not actually one
                if (!mIsMemberDelegate)
                    return false;

                // This is a hack so that the proper has_proc_address function is called to get the return value
                MemberDelegate<className, returnType, parameters...> *memberDelegateObj = (MemberDelegate<className, returnType, parameters...>*)this;
                return memberDelegateObj->has_procaddress(procAddress);
            }


            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
            virtual bool has_thispointer(void *thisPointer) = 0;

            /**
             *  @brief Invoke the delegate with the given arguments and return a value, if any.
             *  @param params Anything; It depends on the function signature specified in the template.
             *  @return Anything; It depends on the function signature specified in the template.
             *  @throw std::runtime_error Thrown when assertions are disabled and either a MemberDelegate
             *  or StaticDelegate type have a NULL function to call.
             *  @throw std::runtime_error Thrown when assertions are disabled and this is a MemberDelegate
             *  attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the function to be called.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            virtual returnType invoke(parameters... params) = 0;
    };
} // End Namespace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_HPP_
