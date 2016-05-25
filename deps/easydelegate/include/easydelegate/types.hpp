/**
 *  @file types.hpp
 *  @date 5/25/2016
 *  @version 2.2
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

    //! Helper typedef to a pointer of a static method with the given signature.
    template <typename returnType, typename... parameters>
    using StaticMethodPointer = returnType(*)(parameters...);

    //! Helper typedef to a pointer of a class member method with the given signature.
    template <typename className, typename returnType, typename... parameters>
    using MemberMethodPointer = returnType(className::*)(parameters...);
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_TYPES_HPP_
