/**
 *  @file deferredcallers.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Include file declaring the deferred caller types for use in systems where
 *  deferred calls is a necessity.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#if !defined(EASYDELEGATE_NO_DEFERRED_CALLING) && __cplusplus >= 201103L

#ifndef _INCLUDE_EASYDELEGATE_DEFERREDCALLERS_HPP_
#define _INCLUDE_EASYDELEGATE_DEFERREDCALLERS_HPP_

#include <assert.h> // assert(expr)
#include <tuple>    // std::tuple

namespace EasyDelegate
{
    /**
     *  @brief The most generic of the deferred caller types. All deferred callers
     *  eventually trace back to this class in their hierarchy, therefore it is possible
     *  to cast them to this type and use the EasyDelegate::IDeferredCaller::genericDispatch
     *  method.
     */
    class IDeferredCaller
    {
        // Public Methods
        public:
            /**
             *  @brief Invoke the deferred caller and ignore the return value.
             */
            EASYDELEGATE_INLINE virtual void genericDispatch(void) const = 0;

            /**
             *  @brief Returns whether or not this deferred caller calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this deferred caller calls a member function
             *  against the given this pointer.
             *  @note Always returns false for static deferred caller types because they do not use a this pointer.
             */
			EASYDELEGATE_INLINE virtual bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT{ return false; }
    };

    /**
     *  @brief A less generic deferred caller type that retains return type information.
     *  Use this if you don't care about signature information and want to call the methods
     *  and get their return values.
     */
    template <typename returnType>
    class ITypedDeferredCaller : public IDeferredCaller
    {
        // Public Methods
        public:
            /**
             *  @brief Invoke the deferred caller and return the result to the
             *  calling method.
             *  @return The value of the called method's return.
             */
            EASYDELEGATE_INLINE virtual returnType dispatch(void) const = 0;
    };

    template <typename className, typename returnType, typename... parameters>
    class DeferredMemberCaller;

    /**
     *  @brief A deferred caller type for static methods.
     *  @details The DeferredStaticCaller class works by storing the information
     *  required to make a call against a static method in its data structure
     *  when constructed. The parameters are stored in an std::tuple and are
     *  later unpacked when the DeferredStaticCaller is dispatched.
     */
    template <typename returnType, typename... parameters>
    class DeferredStaticCaller : public ITypedDeferredCaller<returnType>
    {
        // Public Methods
        public:
            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateMethodPointer)(parameters...);

            /**
             *  @brief Constructor accepting a pointer to a static method.
             *  @param methodPointer A pointer to the member function to be invoked.
             *  @param params The parameter list to use when later dispatching this DeferredStaticCaller.
             */
            DeferredStaticCaller(const StaticDelegateMethodPointer methodPointer, parameters... params) :
            mParameters(params...), mMethodPointer(methodPointer) { }

            /**
             *  @brief Dispatches the DeferredStaticCaller.
             *  @details This is equivalent to the invoke() method on all other delegate
             *  types except the parameters were cached at creation. Said cached parameters
             *  will be passed in automatically upon calling this, so it is completely safe
             *  to store.
             *  @return Anything; it depends on the method signature defined in the template.
             */
            EASYDELEGATE_INLINE returnType dispatch(void) const
            {
                assert(mMethodPointer);

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                return performCachedCall(typename gens<sizeof...(parameters)>::type());
            }

            /**
             *  @brief Dispatches the DeferredStaticCaller, ignoring the return value.
             *  @details This behaves exactly as the dispatch method above except it does not
             *  care about the return of the called function. This method is also callable on
             *  the IDeferredCaller type, unlike the normal dispatch method.
             */
            EASYDELEGATE_INLINE void genericDispatch(void) const { dispatch(); }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the given static method
             *  address.
             *  @param methodPointer A pointer to a static method to be checked against.
             *  @return A boolean representing whether or not this DeferredStaticCaller calls the given method address.
             */
			EASYDELEGATE_INLINE bool callsMethod(const StaticDelegateMethodPointer methodPointer) const EASYDELEGATE_NOEXCEPT{ return mMethodPointer == methodPointer; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the given static method
             *  address of a differing signature.
             *  @param methodPointer A pointer to a static method to be checked against.
             *  @return A boolean representing whether or not this DeferredStaticCaller calls the given method address.
             *  @note Always returns false because methods of different signatures should reside at their own addresses.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const StaticMethodPointer<otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the given class member method
             *  address.
             *  @param methodPointer A pointer to a class member method to be checked against.
             *  @return A boolean representing whether or not this DeferredStaticCaller calls the given method address.
             *  @note Always returns false because DeferredStaticCaller types cannot call class member methods.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the same method as the specified
             *  DeferredStaticCaller.
             *  @param other A pointer to the DeferredStaticCaller to check against.
             *  @return A boolean representing whether or not this DeferredStaticCaller calls the same method as
             *  the given DeferredStaticCaller.
             */
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredStaticCaller<returnType, parameters...>* other) const EASYDELEGATE_NOEXCEPT { return mMethodPointer == other->mMethodPointer; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the same method as the specified
             *  DeferredMemberCaller.
             *  @param other A pointer to the DeferredMemberCaller to check against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredMemberCaller<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls the same of the given DeferredStaticCaller
             *  of a differing signature.
             *  @param other A pointer to a DeferredStaticCaller to be checked against.
             *  @return A boolean representing whether or not the two DeferredStaticCaller instances call the same method.
             *  @note Always returns false because methods of different signatures should reside at their own addresses.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredStaticCaller<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this DeferredStaticCaller calls a member function
             *  against the given this pointer.
             *  @note Always returns false for DeferredStaticCaller types because they do not use a this pointer.
             */
			EASYDELEGATE_INLINE bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredStaticCaller shares the same this pointer as the given
             *  IDeferredCaller.
             *  @param other A pointer to the IDeferredCaller to compare against.
             *  @return A boolean representing whether or not this DeferredStaticCaller shares the same this pointer
             *  as the given IDeferredCaller.
             *  @note Always returns false for DeferredStaticCaller types because they do not use a this pointer.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const IDeferredCaller* other) const EASYDELEGATE_NOEXCEPT { return false; }

        // Private Methods
        private:
            //! Internal templated method to dispatch the call.
            template<int ...S>
            EASYDELEGATE_INLINE EASYDELEGATE_CONSTEXPR returnType performCachedCall(seq<S...>) const
            {
                return (mMethodPointer)(std::get<S>(mParameters) ...);
            }

            //! An internal pointer to the method to be called.
            const StaticDelegateMethodPointer mMethodPointer;
            //! An internal std::tuple that is utilized to cache the parameter list.
            const NoReferenceTuple<parameters...> mParameters;
    };

    /**
     *  @brief A deferred caller type for class member methods.
     *  @details The DeferredMemberCaller class works by storing the information
     *  required to make a call against a class member method in its data structure
     *  when constructed. The parameters are stored in an std::tuple and are
     *  later unpacked when the DeferredMemberCaller is dispatched.
     *  @warning The DeferredMemberCaller is only valid while the given this pointer
     *  remains valid.
     */
    template <typename classType, typename returnType, typename... parameters>
    class DeferredMemberCaller : public ITypedDeferredCaller<returnType>
    {
        // Public Methods
        public:
            //! Helper typedef referring to a class member method pointer.
            typedef MemberMethodPointer<classType, returnType, parameters...> MemberDelegateMethodPointer;

            /**
             *  @brief Constructor accepting a this pointer and a member method.
             *  @param methodPointer A pointer to the class member method to be invoked upon
             *  the this pointer.
             *  @param thisPointer A pointer to the object instance to be considered this
             *  during invocation.
             *  @param params Whatever parameters are required to later dispatch this DeferredMemberCaller.
             *  @warning The DeferredMemberCaller invocation code has no way of knowing if the this
             *  pointer at any time has been deallocated. The DeferredMemberCaller will cause undefined
             *  behavior and/or segfault upon invocation in that case.
             */
            DeferredMemberCaller(const MemberDelegateMethodPointer methodPointer, classType* thisPointer, parameters... params) :
            mThisPointer(thisPointer), mParameters(params...), mMethodPointer(methodPointer) { }

            /**
             *  @brief Dispatches the DeferredMemberCaller.
             *  @details This is equivalent to the invoke() method on all other delegate
             *  types except the parameters were cached at creation. Said cached parameters
             *  will be passed in automatically upon calling this, so it is completely safe
             *  to store.
             *  @return Anything; it depends on the function signature defined in the template.
             */
            EASYDELEGATE_INLINE returnType dispatch(void) const
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
             *  @brief Dispatches the DeferredMemberCaller, ignoring the return value.
             *  @details This behaves exactly as the dispatch method above except it does not
             *  care about the return of the called function. This method is also callable on
             *  the IDeferredCaller type, unlike the normal dispatch method.
             */
            EASYDELEGATE_INLINE void genericDispatch(void) const { dispatch(); }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the given class member method.
             *  @param methodPointer A pointer to a class member method to be checked against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the given method pointer.
             */
			EASYDELEGATE_INLINE bool callsMethod(const MemberDelegateMethodPointer methodPointer) const EASYDELEGATE_NOEXCEPT { return methodPointer == mMethodPointer; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the given class member method of a differing
             *  signature.
             *  @param methodPointer A pointer to a class member method to be checked against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the given method pointer.
             *  @note Always returns false because methods of differing signatures should reside at different
             *  addresses.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the given static method.
             *  @param methodPointer A pointer to a static method to be checked against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the given static method pointer.
             *  @note Always returns false because DeferredMemberCaller types cannot call static methods.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool callsMethod(const StaticMethodPointer<otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the same method as another
             *  DeferredMemberCaller.
             *  @param other A pointer to the DeferredMemberCaller to check against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the same method as the other
             *  DeferredMemberCaller.
             */
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredMemberCaller<classType, returnType, parameters...>* other) const EASYDELEGATE_NOEXCEPT { return mMethodPointer == other->mMethodPointer; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the same method as another
             *  DeferredMemberCaller of a differing signature.
             *  @param other A pointer to the DeferredMemberCaller to check against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the same method as the other
             *  DeferredMemberCaller of differing signature.
             *  @note Always returns false because methods of differing signatures should reside at different
             *  addresses.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredMemberCaller<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls the same method as DeferredStaticCaller.
             *  @param other A pointer to the DeferredStaticCaller to check against.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls the same method as the
             *  DeferredStaticCaller.
             *  @note Always returns false because DeferredMemberCaller types cannot call static methods.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameMethodAs(const DeferredStaticCaller<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls against the same this pointer as another.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls against the same this
             *  pointer of the other.
             */
            template <typename otherClass, typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const DeferredMemberCaller<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return reinterpret_cast<void*>(mThisPointer) == reinterpret_cast<void*>(other->mThisPointer); }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls against the same this pointer of the
             *  DeferredStaticCaller.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls against the same this
             *  pointer of the other.
             *  @note Always returns false because DeferredStaticCaller types do not use a this pointer.
             */
            template <typename otherReturn, typename... otherParams>
			EASYDELEGATE_INLINE bool hasSameThisPointerAs(const DeferredStaticCaller<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this DeferredMemberCaller calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this DeferredMemberCaller calls a class member method
             *  against the given this pointer.
             */
			EASYDELEGATE_INLINE bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT { return reinterpret_cast<const void*>(mThisPointer) == reinterpret_cast<const void*>(thisPointer); }

        // Public Members
        public:
            //! A pointer to the this object to invoke against.
            classType* mThisPointer;

        // Private Methods
        private:
            //! Internal templated method to invoke the stored delegate instance.
            template<int ...S>
            EASYDELEGATE_INLINE EASYDELEGATE_CONSTEXPR returnType performCachedCall(seq<S...>) const
            {
                assert(mThisPointer);

                if (!mThisPointer)
                    throw InvalidThisPointerException();

                return (mThisPointer->*mMethodPointer)(std::get<S>(mParameters) ...);
            }

        // Private Members
        private:
            //! An internal pointer to the proc address to be called.
            const MemberDelegateMethodPointer mMethodPointer;

            //! Internal std::tuple that is utilized to cache the parameter list.
            const NoReferenceTuple<parameters...> mParameters;
    };
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_DEFERREDCALLERS_HPP_
#endif // EASYDELEGATE_NO_DEFERRED_CALLING
