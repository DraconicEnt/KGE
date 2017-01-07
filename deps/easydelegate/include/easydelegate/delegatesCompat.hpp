/**
 *  @file delegates.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Include file declaring various delegate types.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#if !defined(_INCLUDE_EASYDELEGATE_DELEGATES_HPP_) && __cplusplus < 201103L
#define _INCLUDE_EASYDELEGATE_DELEGATES_HPP_

#include <stdarg.h>
#include <assert.h>         // assert(expr)
#include <functional>

#include "types.hpp"
#include "exceptions.hpp"

namespace EasyDelegate
{
    template <typename returnType, unsigned int paramCount, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
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

    template <typename returnType, unsigned int paramCount, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
    class MemberDelegate;

    template <typename returnType, unsigned int paramCount, typename paramOneType, typename paramTwoType, typename paramThreeType, typename paramFourType, typename paramFiveType>
    class StaticDelegate;

    template <typename paramOneType, typename paramTwoType, typename paramThreeType, typename paramFourType, typename paramFiveType>
    struct CallParameters
    {
        paramOneType mParamOne;
        paramTwoType mParamTwo;
        paramThreeType mParamThree;
        paramFourType mParamFour;
        paramFiveType mParamFive;
    };

    template <unsigned int paramCount>
    struct InvocationResolver
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer();
            }
        };
    };

    template <>
    struct InvocationResolver<1>
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, 1, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer(parameters.mParamOne);
            }
        };
    };

    template <>
    struct InvocationResolver<2>
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, 2, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer(parameters.mParamOne, parameters.mParamTwo);
            }
        };
    };

    template <>
    struct InvocationResolver<3>
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, 3, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer(parameters.mParamOne, parameters.mParamTwo, parameters.mParamThree);
            }
        };
    };

    template <>
    struct InvocationResolver<4>
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, 4, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer(parameters.mParamOne, parameters.mParamTwo, parameters.mParamThree, parameters.mParamFour);
            }
        };
    };

    template <>
    struct InvocationResolver<5>
    {
        template <typename returnType, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
        struct ResolveSignature
        {
            static returnType call(const StaticDelegate<returnType, 5, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* delegate, CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                return delegate->mMethodPointer(parameters.mParamOne, parameters.mParamTwo, parameters.mParamThree, parameters.mParamFour, parameters.mParamFive);
            }
        };
    };

    /**
     *  @brief A delegate of a static method.
     *  @details A static delegate allows you to create a sort of handle to a given
     *  static method. This handle can then be used to invoke the method anonymously.
     */
    template <typename returnType, unsigned int paramCount, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
    class StaticDelegate : public ITypedDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>
    {
        public:
            typedef typename EasyDelegate::StaticMethodPointer<paramCount>::template Builder<returnType, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>::pointerType MethodPointer;

            /**
             *  @brief Constructor accepting a static method.
             *  @param methodPointer The address of the static method that this delegate
             *  is intended to invoke.
             */
            StaticDelegate(const MethodPointer methodPointer) :
            ITypedDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>(false), mMethodPointer(methodPointer)
            {
            }

            /**
             *  @brief Standard copy constructor.
             *  @param other A pointer to a StaticDelegate with the same method signature to copy.
             */
            StaticDelegate(const StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* other) :
            ITypedDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>(false),
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
            returnType invoke(...)
            {
                assert(mMethodPointer);

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                va_list parameters;
                va_start(parameters, paramCount);

                CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType> builtParameters;
                for (size_t iteration = 0; iteration < paramCount; ++iteration)
                {
                    switch (iteration)
                    {
                        case 0:
                            builtParameters.mParamOne = va_arg(parameters, paramOneType);
                            break;
                        case 1:
                            builtParameters.mParamTwo = va_arg(parameters, paramTwoType);
                            break;
                        case 2:
                            builtParameters.mParamThree = va_arg(parameters, paramThreeType);
                            break;
                        case 3:
                            builtParameters.mParamFour = va_arg(parameters, paramFourType);
                            break;
                        case 4:
                            builtParameters.mParamFive = va_arg(parameters, paramFiveType);
                            break;
                    }
                }

                va_end(parameters);

                return InvocationResolver<paramCount>::template ResolveSignature<returnType, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>::call(this, builtParameters);
            }

            returnType invokeParameters(CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters)
            {
                assert(mMethodPointer);

                if (!mMethodPointer)
                    throw InvalidMethodPointerException();

                // FIXME: We should call va_end before we return
                return InvocationResolver<paramCount>::template ResolveSignature<returnType, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>::call(this, parameters);
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
           // template <typename otherClass, typename otherReturn, typename... otherParams>
			// EASYDELEGATE_INLINE bool callsMethod(const MemberMethodPointer<otherClass, otherReturn, otherParams...> methodPointer) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate calls the same method as the
             *  StaticDelegate to check against.
             */
			EASYDELEGATE_INLINE bool hasSameMethodAs(const StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* other) const EASYDELEGATE_NOEXCEPT { return other->mMethodPointer == mMethodPointer; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @note Always returns false because a static method and a member method should never
             *  share the same address.
             */
         //   template <typename otherClass, typename otherReturn, typename... otherParams>
			//EASYDELEGATE_INLINE bool hasSameMethodAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate calls against the same method as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @note Always returns false because this variant of hasSameMethodAs will be called for all other
             *  StaticDelegate types whose method signatures do not match our own. Thus, we can't possibly be
             *  sharing the same method.
             */
          //  template <typename otherReturn, typename... otherParams>
			//EASYDELEGATE_INLINE bool hasSameMethodAs(const StaticDelegate<otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the MemberDelegate
             *  we are checking against.
             *  @param other The other MemberDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
          //  template <typename otherClass, typename otherReturn, typename... otherParams>
		//	EASYDELEGATE_INLINE bool hasSameThisPointerAs(const MemberDelegate<otherClass, otherReturn, otherParams...>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            /**
             *  @brief Returns whether or not this StaticDelegate uses the same this pointer as the StaticDelegate
             *  we are checking against.
             *  @param other The other StaticDelegate pointer to compare against.
             *  @return A boolean representing whether or not this StaticDelegate uses the same this pointer as
             *  the MemberDelegate to check against.
             *  @note Always returns false because StaticDelegates do not use this pointers.
             */
          //  template <typename otherReturn, typename... otherParams>
	// 		EASYDELEGATE_INLINE bool hasSameThisPointerAs(const StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>* other) const EASYDELEGATE_NOEXCEPT { return false; }

            //! Internal pointer to proc address to be called.
            const MethodPointer mMethodPointer;
    };

    /**
     *  @brief Base delegate type.
     *  @details Inheritance from this type allows for both the StaticDelegate
     *  and MemberDelegate instances to be used in the DelegateSet type as it
     *  specifies a common interface for the two types to be sharing.
     */
    template <typename returnType, unsigned int paramCount, typename paramOneType, typename paramTwoType, typename paramThreeType, typename paramFourType, typename paramFiveType>
    class ITypedDelegate : public IDelegate
    {
        // Public Members
        public:
            //! Helper typedef referring to the return type of this delegate.
            typedef returnType ReturnType;
            //! Helper typedef for when building static delegates.
            typedef StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType> StaticDelegateType;
            //! Helper typedef for when building member delegates.
            //template <typename classType>
           // using MemberDelegateType = MemberDelegate<classType, returnType, parameters...>;


            typedef typename EasyDelegate::StaticMethodPointer<paramCount>::template Builder<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>::pointerType StaticMethodPointerType;


            //! Helper typedef referring to a class member method pointer.
           // template <typename classType>
          //  using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

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
		//	virtual bool callsMethod(const StaticMethodPointerType methodPointer) const EASYDELEGATE_NOEXCEPT = 0;

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
             /*
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
            */

            /**
             *  @brief Returns whether or not this delegate calls against the given this pointer.
             *  @param thisPointer A pointer referring to the object of interest.
             *  @return A boolean representing whether or not this delegate calls a member function
             *  against the given this pointer.
             *  @note Always returns false for StaticDelegate types because they do not use a this pointer.
             */
			//virtual bool hasThisPointer(const void* thisPointer) const EASYDELEGATE_NOEXCEPT = 0;

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
            virtual returnType invoke(...) = 0;

            virtual returnType invokeParameters(CallParameters<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>& parameters) = 0;
    };
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_DELEGATES_HPP_
