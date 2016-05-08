/**
 *  @file delegates.hpp
 *  @date 6/25/2015
 *  @version 2.0
 *  @brief Include file declaring various delegate types.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef _INCLUDE_EASYDELEGATE_DELEGATES_HPP_
#define _INCLUDE_EASYDELEGATE_DELEGATES_HPP_

#include <assert.h>         // assert(expr)

#include "types.hpp"
#include "exceptions.hpp"

namespace EasyDelegate
{
    template <typename returnType, typename... parameters>
    class ITypedDelegate;

    /**
     *  @brief A type that can represent any delegate type, but it cannot be invoked
     *  without casting to a delegate type that knows the proper method signature.
     *  @details This delegate type is useful in the event you wish to pass
     *  around delegates regardless of the type in a system where the method
     *  signature is resolved via other means for type casting and invocation.
     *
     *  This class type has one boolean readily available that can be used to help
     *  distinguish the actual type at runtime: mIsMemberDelegate.
     */
    class IDelegate
    {
        // Public Members
        public:
            //! A boolean representing whether or not this delegate is a member delegate.
            const bool mIsMemberDelegate;

        // Protected Methods
        protected:
            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate
             *  is an instance of MemberDelegate.
             */
			IDelegate(const bool& isMemberDelegate) EASYDELEGATE_NOEXCEPT : mIsMemberDelegate(isMemberDelegate) { }
    };

    template <typename className, typename returnType, typename... parameters>
    class MemberDelegate;

    /**
     *  @brief A delegate of a static method.
     *  @details A static delegate allows you to create a sort of handle to a given
     *  static method. This handle can then be used to invoke the method anonymously.
     */
    template <typename returnType, typename... parameters>
    class StaticDelegate : public ITypedDelegate<returnType, parameters...>
    {
        public:
            //! Helper typedef referring to a static method pointer that is compatible with this delegate.
            typedef StaticMethodPointer<returnType, parameters...> MethodPointer;

            /**
             *  @brief Constructor accepting a static method.
             *  @param methodPointer The address of the static method that this delegate
             *  is intended to invoke.
             */
            StaticDelegate(const MethodPointer methodPointer) : ITypedDelegate<returnType, parameters...>(false), mMethodPointer(methodPointer) { }

            /**
             *  @brief Standard copy constructor.
             *  @param other A pointer to a StaticDelegate with the same method signature to copy.
             */
            StaticDelegate(const StaticDelegate<returnType, parameters...>* other) : ITypedDelegate<returnType, parameters...>(false),
            mMethodPointer(other->mMethodPointer) { }

            /**
             *  @brief Invokes the StaticDelegate.
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
			EASYDELEGATE_INLINE bool hasThisPointer(const void *thisPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the given static method.
             *  @param methodPointer A pointer to the static method to be checked against.
             *  @return A boolean representing whether or not this StaticDelegate calls the given static method.
             */
			EASYDELEGATE_INLINE bool callsMethod(const MethodPointer methodPointer) const EASYDELEGATE_NOEXCEPT { return methodPointer == mMethodPointer; };

            /**
             *  @brief Returns whether or not this StaticDelegate calls the given class member method.
             *  @param methodPointer A pointer to the class member method to be checked against.
             *  @return A boolean representing whether or not this StaticDelegate calls the given class member method.
             *  @note Always returns false because StaticDelegates don't call class member methods.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate calls the same method as the
             *  StaticDelegate to check against.
             */
			EASYDELEGATE_INLINE bool hasSameMethodAs(const StaticDelegate<returnType, parameters...>* other) const EASYDELEGATE_NOEXCEPT { return other->mMethodPointer == mMethodPointer; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @note Always returns false because this variant of hasSameMethodAs will be called for all other
             *  StaticDelegate types whose method signatures do not match our own. Thus, we can't possibly be
             *  sharing the same method.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const StaticDelegate<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const StaticDelegate<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

        private:
            //! Internal pointer to proc address to be called.
            const MethodPointer mMethodPointer;
    };

    /**
     *  @brief A delegate of a class member method.
     *  @details The MemberDelegate behaves exactly like the StaticDelegate type
     *  except it works against class member methods.
     */
    template <typename classType, typename returnType, typename... parameters>
    class MemberDelegate : public ITypedDelegate<returnType, parameters...>
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
            MemberDelegate(const MethodPointer methodPointer, classType* thisPointer) : mThisPointer(thisPointer),
            mMethodPointer(methodPointer), ITypedDelegate<returnType, parameters...>(true) { }

            /**
             *  @brief Standard copy constructor.
             *  @param other The other MemberDelegate pointer with the same signature to copy from.
             */
            MemberDelegate(const MemberDelegate<classType, returnType, parameters...>* other) : mThisPointer(other->mThisPointer),
            mMethodPointer(other->mMethodPointer), ITypedDelegate<returnType, parameters...>(true) { }

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
			EASYDELEGATE_INLINE bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT { return mThisPointer == thisPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the given class member method pointer.
             *  @param methodPointer A pointer to a class member method to be checked against.
             *  @return A boolean representing whether or not this MemberDelegate calls the given class method pointer.
             */
			EASYDELEGATE_INLINE bool callsMethod(const MethodPointer methodPointer) const EASYDELEGATE_NOEXCEPT { return mMethodPointer == methodPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the given static method pointer.
             *  @param methodPointer A pointer to a static method to be checked against.
             *  @return A boolean representing whether or not this MemberDelegate calls the given static method pointer.
             *  @note Always returns false because a MemberDelegate will never be calling a static method.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const StaticMethodPointer<otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this MemeberDelegate calls the given static method address.
             *  @param methodPointer A pointer to the static method to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false because a static method cannot be invoked by a MemberDelegate.
             */
			EASYDELEGATE_INLINE bool callsMethod(const typename ITypedDelegate<returnType, parameters...>::StaticMethodPointerType methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the same method as the other MemberDelegate.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not both MemberDelegate instances share the same method.
             */
			EASYDELEGATE_INLINE bool hasSameMethodAs(const MemberDelegate<classType, returnType, parameters...>* other) const EASYDELEGATE_NOEXCEPT { return other->mMethodPointer == mMethodPointer; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the same method as the other of a deferring
             *  signature.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not the two MemberDelegate instances call the same
             *  method.
             *  @note Always returns false because if the method signatures are different, then the
             *  methods should never share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls the same method as the StaticDelegate
             *  type.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not the this MemberDelegate calls the same method
             *  as the StaticDelegate.
             *  @note Always returns false because MemberDelegate types cannot call static methods.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const StaticDelegate<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls against the same this pointer as the
             *  StaticDelegate.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not the this MemberDelegate calls against the same
             *  this pointer as the StaticDelegate.
             *  @note Always returns false because StaticDelegate types do not use this pointers.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const StaticDelegate<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this MemberDelegate calls against the same this pointer as the
             *  other.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not the this MemberDelegate calls against the same
             *  this pointer as the other.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return reinterpret_cast<void*>(mThisPointer) == reinterpret_cast<void*>(other->mThisPointer); }

        // Public Members
        public:
            //! A pointer to the this object.
            classType* mThisPointer;

        private:
            //! An internal pointer to the proc address to be called.
            const MethodPointer mMethodPointer;
    };

    /**
     *  @brief Base delegate type.
     *  @details Inheritance from this type allows for both the StaticDelegate
     *  and MemberDelegate instances to be used in the DelegateSet type as it
     *  specifies a common interface for the two types to be sharing.
     */
    template <typename returnType, typename... parameters>
    class ITypedDelegate : public IDelegate
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

            //! Helper typedef referring to a class member method pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

        // Public Methods
        public:
            /**
             *  @brief Constructor accepting a boolean.
             *  @param isMemberDelegate A boolean representing whether or not this delegate is a
             *  member delegate.
             */
			ITypedDelegate(const bool& isMemberDelegate) EASYDELEGATE_NOEXCEPT : IDelegate(isMemberDelegate) { }

            /**
             *  @brief Returns whether or not this delegate calls the given static method.
             *  @param methodPointer A pointer to the static method to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given proc address.
             *  @note Always returns false for MemeberDelegate types because a MemberDelegate cannot invoke static
             *  functions.
             */
			virtual bool callsMethod(const StaticMethodPointerType methodPointer) const EASYDELEGATE_NOEXCEPT = 0;

            /**
             *  @brief Returns whether or not this delegate calls the given class member method address.
             *  @param methodPointer A pointer to a member function to be checked against.
             *  @return A boolean representing whether or not this delegate calls the given class member method
             *  address.
             *  @details This is not declared as a wholly virtual function because a templated function
             *  cannot be virtual.
             *  @note Always returns false for StaticDelegate types because a StaticDelegate cannot
             *  call member functions.
             */
            template <typename className>
            bool callsMethod(const MemberDelegateFuncPtr<className> methodPointer) const EASYDELEGATE_NOEXCEPT
            {
                // Don't try to check as a MemberDelegate if we're not actually one
                if (!mIsMemberDelegate)
                    return false;

                // This is a hack so that the proper callsMethod function is called to get the return value
                const MemberDelegate<className, returnType, parameters...>* memberDelegateObj = reinterpret_cast<const MemberDelegate<className, returnType, parameters...>*>(this);
                return memberDelegateObj->callsMethod(methodPointer);
            }

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
			virtual bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT = 0;

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
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_DELEGATES_HPP_
