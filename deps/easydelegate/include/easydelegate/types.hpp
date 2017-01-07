/**
 *  @file types.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Include file declaring various helper types used by the EasyDelegate library. They
 *  may also aid programmers using the library as well.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef EASYDELEGATE_NO_DEFERRED_CALLING
    #include <tuple>        // std::tuple<...>
    #include <type_traits>  // std::remove_reference<type>
#endif

#ifndef _INCLUDE_EASYDELEGATE_TYPES_HPP_
#define _INCLUDE_EASYDELEGATE_TYPES_HPP_

namespace EasyDelegate
{
   #ifndef EASYDELEGATE_NO_DEFERRED_CALLING
    /**
     *  @brief Part of a helper template that is used to statically unpack tuples for
     *  the deferring calling system.
     *
     *  @details Taken from the chosen answer of http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
     */
    template<int ...> struct seq {};

    /**
     *  @brief Part of a helper template that is used to statically unpack tuples for
     *  the deferring calling system.
     *
     *  @details Taken from the chosen answer of http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
     */
    template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

    /**
     *  @brief Part of a helper template that is used to statically unpack tuples for
     *  the deferring calling system.
     *
     *  @details Taken from the chosen answer of http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
     */
    template<int ...S> struct gens<0, S...>
    {
        //! Helper in the expansion logic.
        typedef seq<S...> type;
    };
    #endif

    #ifndef EASYDELEGATE_NO_DEFERRED_CALLING
    /**
     *  @brief A helper typedef for an std::tuple that removes the reference from referenced types.
     *
     *  @details Taken from the chosen answer of http://stackoverflow.com/questions/12742877/remove-reference-from-stdtuple-members
     */
    template <typename... typenames>
    using NoReferenceTuple = std::tuple<typename std::remove_reference<typenames>::type...>;
    #endif // EASYDELEGATE_NO_DEFERRED_CALLING

    #if __cplusplus >= 201103L
    //! Helper typedef to a pointer of a static method with the given signature.
    template <typename returnType, typename... parameters>
    using StaticMethodPointer = returnType(*)(parameters...);

    //! Helper typedef to a pointer of a class member method with the given signature.
    template <typename className, typename returnType, typename... parameters>
    using MemberMethodPointer = returnType(className::*)(parameters...);
    #else

    template <unsigned int paramCount>
    struct StaticMethodPointer
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>
        struct Builder
        {
            typedef returnType (*pointerType)(void);
        };
    };

    template <>
    struct StaticMethodPointer<1>
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>
        struct Builder
        {
            typedef returnType (*pointerType)(paramTypeOne);
        };
    };

    template <>
    struct StaticMethodPointer<2>
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>
        struct Builder
        {
            typedef returnType (*pointerType)(paramTypeOne, paramTypeTwo);
        };
    };

    template <>
    struct StaticMethodPointer<3>
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>

        struct Builder
        {
            typedef returnType (*pointerType)(paramTypeOne, paramTypeTwo, paramTypeThree);
        };
    };

    template <>
    struct StaticMethodPointer<4>
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>
        struct Builder
        {
            typedef returnType (*pointerType)(paramTypeOne, paramTypeTwo, paramTypeThree, paramTypeFour);
        };
    };

    template <>
    struct StaticMethodPointer<5>
    {
        template <typename returnType, typename paramTypeOne = int, typename paramTypeTwo = int, typename paramTypeThree = int, typename paramTypeFour = int, typename paramTypeFive = int>
        struct Builder
        {
            typedef returnType (*pointerType)(paramTypeOne, paramTypeTwo, paramTypeThree, paramTypeFour, paramTypeFive);
        };
    };
    #endif
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_TYPES_HPP_
