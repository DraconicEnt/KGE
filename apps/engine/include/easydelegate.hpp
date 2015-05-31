/**
 *  @file easydelegate.hpp
 *  @version 2.0
 *  @date 5/2/2015
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *  @brief Portable delegate system that should work on any C++11 compliant compiler.
 *  @details EasyDelegate is a library that exposes an easy and flexible delegate system with
 *  the use of C++11's variatic templates.
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef _INCLUDE_EASYDELEGATE_HPP_
#define _INCLUDE_EASYDELEGATE_HPP_

#include <stdexcept>        // Standard exception types
#include <assert.h>         // assert(expr)
#include <type_traits>      // std::remove_reference<type>

#include <set>              // std::set<type>
#include <tuple>            // std::tuple<...>
#include <unordered_set>    // std::unordered_set<type>

// Define __forceinline if we're on GCC
#if defined(__GNUC__) || defined(__GNUG__)
    #define __forceinline __attribute__((always_inline))
#endif

// If we're going to inline stuff, force it
#ifdef EASYDELEGATE_FORCE_INLINE
    #define INLINE __forceinline
#else
    //! A preprocessor definition for a keyword that forces the inlining of a given method.
    #define INLINE
#endif

//! Namespace containing all EasyDelegate functionality.
namespace EasyDelegate
{
    using namespace std;

    // Taken from the chosen answer of
    // http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
    template<int ...> struct seq {};
    template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};
    template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

    /**
     *  @brief A helper typedef for an std::tuple that removes the reference from referenced types.
     *
     *  @detail Taken from the chosen answer of http://stackoverflow.com/questions/12742877/remove-reference-from-stdtuple-members
     */
    template <typename... typenames>
    using NoReferenceTuple = tuple<typename remove_reference<typenames>::type...>;

    //! Helper typedef to a pointer of a static method with the given signature.
    template <typename returnType, typename... parameters>
    using StaticMethodPointer = returnType(*)(parameters...);

    //! Helper typedef to a pointer of a member method with the given signature.
    template <typename className, typename returnType, typename... parameters>
    using MemberMethodPointer = returnType(className::*)(parameters...);

    //! Forward declaration to a CachedStaticDelegate, to keep this lib in a single file.
    template <typename returnType, typename... parameters>
    class CachedStaticDelegate;

    //! Forward declaration to a CachedMemberDelegate, to keep this lib in a single file.
    template <typename classType, typename returnType, typename... parameters>
    class CachedMemberDelegate;

    //! Forward declaration of the DelegateBase type, to help keep this lib in a single file.
    template <typename returnType, typename... parameters>
    class DelegateBase;

    //! Forward declaration of the MemberDelegate type, to help keep this lib in a single file.
    template <typename classType, typename returnType, typename... parameters>
    class MemberDelegate;

    /**
     *  @brief A general base exception type for all exceptions that is thrown
     *  by the EasyDelegate library.
     */
    class DelegateException : public exception { };

    /**
     *  @brief An exception type that is thrown by the EasyDelegate library
     *  when a member delegate type has attempted to make a call using a
     *  NULL 'this' pointer.
     */
    class InvalidThisPointerException : public DelegateException
    {
        // Public Methods
        public:
            /**
             *  @brief Returns a pointer to the exception text from the
             *  exception.
             *  @return A pointer to the exception text in this exception.
             */
            virtual const char *what() const throw()
            {
                return "Attempted to call a class member method against a NULL 'this' pointer";
            }
    };

    /**
     *  @brief An exception type that is thrown by the EasyDelegate library when
     *  any delegate type has attempted to make a call against a NULL method
     *  pointer.
     */
    class InvalidMethodPointerException : public DelegateException
    {
        // Public Methods
        public:
            /**
             *  @brief Returns a pointer to the exception text from the
             *  exception.
             *  @return A pointer to the exception text in this exception.
             */
            virtual const char *what() const throw()
            {
                return "Attempted to perform a call against a NULL method pointer";
            }
    };

    /**
     *  @brief A type that can represent any delegate type, but it cannot be invoked
     *  without casting to a delegate type that knows the proper method signature.
     *  @details This delegate type is useful in the event you wish to pass
     *  around delegates regardless of the type in a system where the method
     *  signature is resolved via other means for type casting and invocation.
     *
     *  This class type has two booleans readily available that can be used to help
     *  distinguish the actual type at runtime: mIsMemberDelegate and mIsCachedDelegate.
     */
    class GenericDelegate
    {
        // Public Members
        public:
            //! A boolean representing whether or not this delegate is a member delegate.
            const bool mIsMemberDelegate;
            //! A boolean representing whether or not this delegate is a cached delegate.
            const bool mIsCachedDelegate;

        // Protected Methods
        protected:
            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate
             *  is an instance of MemberDelegate.
             */
            GenericDelegate(const bool &isMemberDelegate, const bool &isCachedDelegate) noexcept :
            mIsMemberDelegate(isMemberDelegate), mIsCachedDelegate(isCachedDelegate) { }
    };

    /**
     *  @brief A delegate of a static method.
     *  @details A static delegate allows you to create a sort of handle to a given
     *  static method. This handle can then be used to invoke the method anonymously.
     */
    template <typename returnType, typename... parameters>
    class StaticDelegate : public DelegateBase<returnType, parameters...>
    {
        public:
            //! Helper typedef referring to a static method pointer that is compatible with this delegate.
            typedef StaticMethodPointer<returnType, parameters...> MethodPointer;

            /**
             *  @brief Constructor accepting a static method.
             *  @param methodPointer The address of the static method that this delegate
             *  is intended to invoke.
             */
            StaticDelegate(const StaticDelegate::MethodPointer methodPointer) : DelegateBase<returnType, parameters...>(false), mMethodPointer(methodPointer) { }

            /**
             *  @brief Standard copy constructor.
             *  @param other A pointer to a StaticDelegate with the same method signature to copy.
             */
            StaticDelegate(const StaticDelegate<returnType, parameters...> *other) : DelegateBase<returnType, parameters...>(false),
            mMethodPointer(other->mMethodPointer) { }

            /**
             *  @brief Invoke the StaticDelegate.
             *  @param params Anything; It depends on the method signature specified in the template.
             *  @return Anything; It depends on the function signature specified in the template.
             *  @throw EasyDelegate::InvalidMethodPointerException Thrown when the static method that this
             *  delegate is supposed to be invoking is NULL.
             *  @throw std::exception Potentially thrown by the method called by
             *  this StaticDelegate.
             *  @note The call will not throw an exception in any of the EasyDelegate::DelegateException cases
             *  but rather assert if assertions are enabled.
             */
            returnType invoke(parameters... params)
            {
                assert(mMethodPointer);

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                return ((MethodPointer)mMethodPointer)(params...);
            }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this StaticDelegate calls a member method
             *  against the given this pointer.
             *  @note Always returns false because StaticDelegates don't use a this pointer.
             */
            bool hasThisPointer(const void *thisPointer) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the given static method.
             *  @param methodPointer A pointer to the static method to be checked against.
             *  @return A boolean representing whether or not this StaticDelegate calls the given static method.
             */
            bool callsMethod(const StaticDelegate::MethodPointer methodPointer) const noexcept { return methodPointer == mMethodPointer; };

            /**
             *  @brief Returns whether or not this StaticDelegate calls the given class member method.
             *  @param methodPointer A pointer to the class member method to be checked against.
             *  @return A boolean representing whether or not this StaticDelegate calls the given class member method.
             *  @note Always returns false because StaticDelegates don't call class member methods.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate calls the same method as the
             *  StaticDelegate to check against.
             */
            bool hasSameMethodAs(const StaticDelegate<returnType, parameters...> *other) const noexcept { return other->mMethodPointer == mMethodPointer; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const MemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
            template <typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const StaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const MemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false;  }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @note Always returns false because this variant of hasSameMethodAs will be called for all other
             *  StaticDelegate types whose method signatures do not match our own. Thus, we can't possibly be
             *  sharing the same method.
             */
            template <typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const StaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

        private:
            //! Internal pointer to proc address to be called.
            const StaticDelegate::MethodPointer mMethodPointer;
    };

    /**
     *  @brief A delegate of a class member method.
     *  @details The MemberDelegate behaves exactly like the StaticDelegate type
     *  except it works against class member methods.
     */
    template <typename classType, typename returnType, typename... parameters>
    class MemberDelegate : public DelegateBase<returnType, parameters...>
    {
        public:
            //! Helper typedef referring to a static function pointer that is compatible with this delegate.
            typedef MemberMethodPointer<classType, returnType, parameters...> MethodPointer;

            /**
             *  @brief Constructor accepting a this pointer and a member function.
             *  @param thisPointer A pointer to the object instance to be considered 'this'
             *  during invocation.
             *  @param methodPointer A pointer to the member function to be invoked upon
             *  the this pointer.
             *  @warning The MemberDelegate invocation code has no way of knowing if the 'this'
             *  pointer at any time has been deallocated. The MemberDelegate will cause undefined
             *  behavior and/or segfault upon invocation in that case.
             */
            MemberDelegate(const MemberDelegate::MethodPointer methodPointer, classType *thisPointer) : mThisPointer(thisPointer),
            mMethodPointer(methodPointer), DelegateBase<returnType, parameters...>(true) { }

            /**
             *  @brief Standard copy constructor.
             *  @param other The other MemberDelegate pointer with the same signature to copy from.
             */
            MemberDelegate(const MemberDelegate<classType, returnType, parameters...> *other) : mThisPointer(other->mThisPointer),
            mMethodPointer(other->mMethodPointer), DelegateBase<returnType, parameters...>(true) { }

            /**
             *  @brief Invoke the MemberDelegate.
             *  @param params Anything; It depends on the method signature specified in the template.
             *  @return Anything; It depends on the method signature specified in the template.
             *  @throw EasyDelegate::InvalidMethodPointerException Thrown when the class member method that this
             *  delegate is supposed to be invoking is NULL.
             *  @throw EasyDelegate::InvalidThisPointerException Thrown when the MemberDelegate's this pointer
             *  is NULL.
             *  @throw std::exception Potentially thrown by the method called by this MemberDelegate.
             *  @note The call will not throw an exception in any of the EasyDelegate::DelegateException cases
             *  but rather assert if assertions are enabled.
             */
            returnType invoke(parameters... params)
            {
                assert(mThisPointer);
                assert(mMethodPointer);

                if (!mThisPointer)
                    throw InvalidThisPointerException();

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                classType *thisPointer = (classType*)thisPointer;
                return (thisPointer->*mMethodPointer)(params...);
            }

            /**
             *  @brief Returns whether or not this MemberDelegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this MemberDelegate calls a member function
             *  against the given this pointer.
             */
            bool hasThisPointer(const void *thisPointer) const noexcept { return mThisPointer == thisPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the given class member method pointer.
             *  @param methodPointer A pointer to a class member method to be checked against.
             *  @return A boolean representing whether or not this MemberDelegate calls the given class method pointer.
             */
            bool callsMethod(const MemberDelegate::MethodPointer methodPointer) const noexcept { return mMethodPointer == methodPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the given static method pointer.
             *  @param methodPointer A pointer to a static method to be checked against.
             *  @return A boolean representing whether or not this MemberDelegate calls the given static method pointer.
             *  @note Always returns false because a MemberDelegate will never be calling a static method.
             */
            template <typename otherReturn, typename... otherParams>
            bool callsMethod(const StaticMethodPointer<otherReturn, otherParams...> methodPointer) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this MemeberDelegate calls the given static proc address.
             *  @param procAddress A pointer to the static function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false because a static function cannot be invoked by a MemberDelegate.
             */
            bool callsMethod(const typename DelegateBase<returnType, parameters...>::StaticMethodPointerType methodPointer) const noexcept { return false; }

            /**
             *  @brief Equality overload for comparisons against other MemberDelegate references of the same method
             *  signature.
             *  @param other The other MemberDelegate reference to compare against.
             */
            bool hasSameMethodAs(const MemberDelegate<classType, returnType, parameters...> *other) const noexcept { return other->mMethodPointer == mMethodPointer; }

            template <typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const StaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const MemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return (void*)mThisPointer == (void*)other->mThisPointer; }

            /**
             *  @brief Equality overload for comparisons against MemberDelegate references that do not
             *  share the same method signature as this MemberDelegate.
             *  @param other The other MemberDelegate reference to compare against.
             *  @note Always returns false because if the method signatures are different, then the
             *  methods should never share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const MemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false;  }

            /**
             *  @brief Equality overload for comparisons against a reference of a StaticDelegate.
             *  @param other The StaticDelegate reference to compare against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
            template <typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const StaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

        // Public Members
        public:
            //! A pointer to the this object.
            classType *mThisPointer;

        private:
            //! An internal pointer to the proc address to be called.
            const MemberDelegate::MethodPointer mMethodPointer;
    };

    /**
     *  @brief A set of delegate instances that provides helper methods to invoke all
     *  contained delegates.
     *  @details The DelegateSet type can be described as a sink for specific event types.
     *  Typical usage of this behavior would involve creating a typedef of a DelegateSet type
     *  for a specific method signature which then has its own specialized typedefs to facilitate
     *  the creation of StaticDelegate and MemberDelegate types that are compatible with instances
     *  of this new specialized DelegateSet type.
     *  @example ex_events.cpp
     */
    template <typename returnType, typename... parameters>
    class DelegateSet : public set<DelegateBase<returnType, parameters...> *>
    {
        public:
            //! Helper typedef to construct the function pointer signature from the template.
            typedef returnType (*delegateFuncPtr)(parameters...);

        public:
            //! Helper typedef for when building static delegates for this set.
            typedef StaticDelegate<returnType, parameters...> StaticDelegateType;
            //! Helper typedef for when building member delegates for this set.
            template <typename classType>
            using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;
            //! Helper typedef for when wanting the return type of this set.
            typedef returnType ReturnType;
            //! Helper typedef to construct the StaticDelegate signature from the template.
            typedef DelegateBase<returnType, parameters...> DelegateBaseType;

            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

            template <typename classType>
            using CachedMemberDelegateType = CachedMemberDelegate<classType, returnType, parameters...>;

            typedef CachedStaticDelegate<returnType, parameters...> CachedStaticDelegateType;

            typedef set<returnType> ReturnSetType;

            //! Standard destructor.
            ~DelegateSet(void)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    delete *it;
            }

            /**
             *  @brief Invoke all delegates in the set, ignoring return values.
             *  @param args All other arguments that will be used as parameters to each delegate.
             *  @throw InvalidMethodPointerException Thrown when assertions are disabled and either
             *  a stored MemberDelegate or StaticDelegate type have a NULL function to call.
             *  @throw InvalidThisPointerException Thrown when assertions are disabled and a stored
             *  MemberDelegate is attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the DelegateException cases but rather
             *  assert if assertions are enabled.
             */
            void invoke(parameters... params) const
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    (*it)->invoke(params...);
            }

            /**
             *  @brief Invoke all delegates in the set, storing return values in out.
             *  @param out The std::set that all return values will be sequentially written to.
             *  @param args All other arguments that will be used as parameters to each delegate.
             *  @throw InvalidMethodPointerException Thrown when assertions are disabled and either
             *  a stored MemberDelegate or StaticDelegate type have a NULL function to call.
             *  @throw InvalidThisPointerException Thrown when assertions are disabled and a stored
             *  MemberDelegate is attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the DelegateException cases but rather
             *  assert if assertions are enabled.
             */
            void invoke(set<returnType> &out, parameters... params) const
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    out.insert(out.end(), (*it)->invoke(params...));
            }

            /**
             *  @brief Pushes a delegate instance to the end of the set.
             *  @param delegateInstance The delegate instance to the pushed onto the set.
             */
            void push_back(DelegateSet::DelegateBaseType *delegateInstance)
            {
                this->insert(this->end(), delegateInstance);
            }

            //! Alternate to push_back that looks a bit prettier in source.
            void operator +=(DelegateSet::DelegateBaseType *delegateInstance)
            {
                this->push_back(delegateInstance);
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
            template <typename className>
            void removeDelegateByMethod(DelegateSet::MemberDelegateFuncPtr<className> procAddress, bool deleteInstances=true, unordered_set<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    DelegateSet::DelegateBaseType *current = *it;

                    if (current->callsMethod(procAddress))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->insert(out->end(), current);

                        this->erase(current);
                    }
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
            void removeDelegateByMethod(DelegateSet::StaticDelegateFuncPtr methodPointer, const bool &deleteInstances=true, unordered_set<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    DelegateSet::DelegateBaseType *current = *it;

                    if (current->callsMethod(methodPointer))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->insert(out->end(), current);

                        this->erase(current);
                    }
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
            void removeDelegateByThisPointer(const void *thisPtr, const bool &deleteInstances=true, unordered_set<DelegateSet::DelegateBaseType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    DelegateSet::DelegateBaseType *current = *it;

                    if (current->mIsMemberDelegate && current->hasThisPointer(thisPtr))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->insert(out->end(), current);

                        this->erase(current);
                    }
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
            DelegateSet::DelegateBaseType *removeDelegate(DelegateSet::DelegateBaseType *instance, const bool &deleteInstance=true)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    DelegateSet::DelegateBaseType *current = *it;

                    if (current == instance)
                    {
                        if (deleteInstance)
                            delete current;

                        this->erase(current);
                        return current;
                    }
                }

                return NULL;
            }
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
        // Public Members
        public:
            //! Helper typedef referring to the return type of this delegate.
            typedef returnType ReturnType;
            //! Helper typedef for when building static delegates.
            typedef StaticDelegate<returnType, parameters...> StaticDelegateType;
            //! Helper typedef for when building member delegates.
            template <typename classType>
            using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;

            //! Helper typedef referring to a static function pointer.
            typedef StaticMethodPointer<returnType, parameters...> StaticMethodPointerType;

            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

        // Public Methods
        public:
            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate is a
             *  member delegate.
             */
            DelegateBase(const bool &isMemberDelegate) noexcept : GenericDelegate(isMemberDelegate, false) { }

            /**
             *  @brief Returns whether or not this delegate calls the given static proc address.
             *  @param procAddress A pointer to the static function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false for MemeberDelegate types because a MemberDelegate cannot invoke static
             *  functions.
             */
            virtual bool callsMethod(const DelegateBase::StaticMethodPointerType methodPointer) const noexcept = 0;

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
            bool callsMethod(const DelegateBase::MemberDelegateFuncPtr<className> procAddress) const noexcept
            {
                // Don't try to check as a MemberDelegate if we're not actually one
                if (!mIsMemberDelegate)
                    return false;

                // This is a hack so that the proper callsMethod function is called to get the return value
                MemberDelegate<className, returnType, parameters...> *memberDelegateObj = (MemberDelegate<className, returnType, parameters...>*)this;
                return memberDelegateObj->callsMethod(procAddress);
            }

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
            virtual bool hasThisPointer(const void *thisPointer) const noexcept = 0;

            /**
             *  @brief Invoke the delegate with the given arguments and return a value, if any.
             *  @param params Anything; It depends on the function signature specified in the template.
             *  @return Anything; It depends on the function signature specified in the template.
             *  @throw InvalidMethodPointerException Thrown when assertions are disabled and either a MemberDelegate
             *  or StaticDelegate type have a NULL function to call.
             *  @throw InvalidThisPointerException Thrown when assertions are disabled and this is a MemberDelegate
             *  attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the function to be called.
             *  @note The call will not throw an exception in any of the std::runtime_error cases but rather
             *  assert if assertions are enabled.
             */
            virtual returnType invoke(parameters... params) = 0;
    };

    /**
     *  @brief The most generic of the cached delegate types. All cached delegate
     *  eventually trace back to this class in their hierarchy, therefore it is possible
     *  to cast them to this type and use the EasyDelegate::GenericCachedDelegate::genericDispatch
     *  method.
     */
    class GenericCachedDelegate
    {
        // Public Methods
        public:
            /**
             *  @brief Invoke the cached delegate and ignore the return value.
             */
            virtual void genericDispatch(void) const = 0;

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
            virtual bool hasThisPointer(const void *thisPointer) const noexcept = 0;
    };

    /**
     *  @brief
     */
    template <typename returnType>
    class GenericTypedCachedDelegate : public GenericCachedDelegate
    {
        // Public Methods
        public:
            /**
             *  @brief Invoke the cached delegate and return the result to the
             *  calling method.
             *  @return The value of the called method's return.
             */
            virtual returnType dispatch(void) const = 0;

            /**
             *  @brief Invoke the cached delegate and ignore the return value.
             */
            virtual void genericDispatch(void) const = 0;
    };

    /**
     *  @brief A delegate type for class member methods that facilitates deferred
     *  calls.
     *  @detail The CachedMemberDelegate class works by storing the information
     *  required to make a call against a class member method in its data structure
     *  when constructed. The parameters are stored in an std::tuple and are
     *  later unpacked when the CachedMemberDelegate is dispatched.
     *  @note The CachedMemberDelegate is valid throughout its own lifetime and the
     *  associated "this" object's lifetime.
     */
    template <typename classType, typename returnType, typename... parameters>
    class CachedMemberDelegate : public GenericTypedCachedDelegate<returnType>
    {
        // Public Methods
        public:
            //! Helper typedef referring to a member function pointer.
            typedef MemberMethodPointer<classType, returnType, parameters...> MemberDelegateMethodPointer;

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
            CachedMemberDelegate(const MemberDelegateMethodPointer methodPointer, classType *thisPointer, parameters... params) :
            mThisPointer(thisPointer), mParameters(params...), mMethodPointer(methodPointer)
            {

            }

            /**
             *  @brief Dispatches the CachedDelegate.
             *  @details This is equivalent to the invoke() method on all other delegate
             *  types except the parameters were cached at creation. Said cached parameters
             *  will be passed in automatically upon calling this, so it is completely safe
             *  to store.
             *  @return Anything; it depends on the function signature defined in the template.
             */
            returnType dispatch(void) const
            {
                assert(mThisPointer);
                assert(mMethodPointer);

                if (!mThisPointer)
                    throw InvalidThisPointerException();

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                return performCachedCall(typename gens<sizeof...(parameters)>::type());
            }

            /**
             *  @brief Dispatches the CachedDelegate, ignoring the return value.
             *  @details This behaves exactly as the dispatch method above except it does not
             *  care about the return of the called function. This method is also callable on
             *  the CachedDelegateBase type, unlike the normal dispatch method.
             */
            void genericDispatch(void) const { dispatch(); }

            /**
             *  @brief Returns whether or not this delegate calls the given member proc address.
             *  @param procAddress A pointer to a member function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @details This is not declared as a wholly virtual function because a templated function
             *  cannot be virtual.
             *  @note Always returns false for StaticDelegate types because a StaticDelegate cannot
             *  call member functions.
             */
            bool callsMethod(const MemberDelegateMethodPointer methodPointer) const noexcept { return methodPointer == mMethodPointer; }

            template <typename otherReturn, typename... otherParams>
            bool callsMethod(const StaticMethodPointer<otherReturn, otherParams...> methodPointer) const noexcept { return false; }

            bool hasSameMethodAs(const CachedMemberDelegate<classType, returnType, parameters...> *other) const noexcept { return mMethodPointer == other->mMethodPointer; }

            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const CachedMemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false; }

            template <typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const CachedStaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const CachedMemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return (void*)mThisPointer == (void*)other->mThisPointer; }

            template <typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const CachedStaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
            bool hasThisPointer(const void *thisPointer) const noexcept { return mThisPointer == thisPointer; }

        // Public Members
        public:
            //! A pointer to the this object to invoke against.
            classType *mThisPointer;

        // Private Methods
        private:
            //! Internal templated method to invoke the stored delegate instance.
            template<int ...S>
            INLINE returnType performCachedCall(seq<S...>) const
            {
                assert(mThisPointer);

                if (!mThisPointer)
                    throw InvalidThisPointerException();

                return (mThisPointer->*mMethodPointer)(get<S>(mParameters) ...);
            }

        // Private Members
        private:
            //! An internal pointer to the proc address to be called.
            const MemberDelegateMethodPointer mMethodPointer;

            //! Internal std::tuple that is utilized to cache the parameter list.
            const NoReferenceTuple<parameters...> mParameters;
    };

    /**
     *  @brief A delegate type for static methods that facilitates deferred calls.
     *  @detail The CachedStaticDelegate class works by storing the information
     *  required to make a call against a static method in its data structure
     *  when constructed. The parameters are stored in an std::tuple and are
     *  later unpacked when the CachedStaticDelegate is dispatched.
     */
    template <typename returnType, typename... parameters>
    class CachedStaticDelegate : public GenericTypedCachedDelegate<returnType>
    {
        // Public Methods
        public:
            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateMethodPointer)(parameters...);

            /**
             *  @brief Constructor accepting a this pointer and a member function.
             *  @param methodPointer A pointer to the member function to be invoked upon
             *  'this'.
             *  @param params The parameter list to use when later dispatching this StaticDelegate.
             *  @note The MemberDelegate invocation code has no way of knowing if the 'this'
             *  pointer at any time has been deallocated. The MemberDelegate will cause undefined
             *  behavior and/or segfault upon invocation in that case.
             */
            CachedStaticDelegate(const StaticDelegateMethodPointer methodPointer, parameters... params) :
            mParameters(params...), mMethodPointer(methodPointer) { }

            /**
             *  @brief Dispatches the CachedDelegate.
             *  @details This is equivalent to the invoke() method on all other delegate
             *  types except the parameters were cached at creation. Said cached parameters
             *  will be passed in automatically upon calling this, so it is completely safe
             *  to store.
             *  @return Anything; it depends on the function signature defined in the template.
             */
            INLINE returnType dispatch(void) const
            {
                assert(mMethodPointer);

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                return performCachedCall(typename gens<sizeof...(parameters)>::type());
            }

            /**
             *  @brief Dispatches the CachedDelegate, ignoring the return value.
             *  @details This behaves exactly as the dispatch method above except it does not
             *  care about the return of the called function. This method is also callable on
             *  the CachedDelegateBase type, unlike the normal dispatch method.
             */
            void genericDispatch(void) const { dispatch(); }

            /**
             *  @brief Returns whether or not this delegate calls the given member proc address.
             *  @param procAddress A pointer to a member function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @details This is not declared as a wholly virtual function because a templated function
             *  cannot be virtual.
             *  @note Always returns false for StaticDelegate types because a StaticDelegate cannot
             *  call member functions.
             */
            bool callsMethod(const StaticDelegateMethodPointer methodPointer) const noexcept { return mMethodPointer == methodPointer; }

            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const noexcept { return false; }

            /**
             *  @brief Equality overload for comparisons against other CachedStaticDelegate references of
             *  the same method signature.
             *  @param other The other CachedStaticDelegate reference to compare against.
             */
            bool hasSameMethodAs(const CachedStaticDelegate<returnType, parameters...> *other) const noexcept { return mMethodPointer == other->mMethodPointer; }

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
            bool hasThisPointer(const void *thisPointer) const noexcept { return false; }

            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const CachedMemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false; }

            template <typename otherReturn, typename... otherParams>
            bool hasSameThisPointerAs(const CachedStaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

            /**
             *  @brief Equality overload for comparisons against CachedMemberDelegate references.
             *  @param other The CachedMemberDelegate reference to compare against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const CachedMemberDelegate<otherClass, otherReturn, otherParams...> *other) const noexcept { return false; }

            /**
             *  @brief Equality overload for comparisons against CachedStaticDelegate references that
             *  do not share the same method signature as this CachedStaticDelegate.
             *  @param other The other CachedStaticDelegate reference to compare against.
             *  @note Always returns false because if the method signatures are different, then the
             *  methods should never share the same address.
             */
            template <typename otherReturn, typename... otherParams>
            bool hasSameMethodAs(const CachedStaticDelegate<otherReturn, otherParams...> *other) const noexcept { return false; }

        // Private Methods
        private:
            //! Internal templated method to invoke the stored delegate instance.
            template<int ...S>
            INLINE constexpr returnType performCachedCall(seq<S...>) const
            {
                return (mMethodPointer)(get<S>(mParameters) ...);
            }

            //! An internal pointer to the method to be called.
            const StaticDelegateMethodPointer mMethodPointer;
            //! An internal std::tuple that is utilized to cache the parameter list.
            const NoReferenceTuple<parameters...> mParameters;
    };
} // End Namespace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_HPP_
