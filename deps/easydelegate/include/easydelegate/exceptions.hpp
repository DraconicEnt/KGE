/**
 *  @file exceptions.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Include file containing the definitions for all exception types thrown by the
 *  EasyDelegate library.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef _INCLUDE_EASYDELEGATE_EXCEPTIONS_HPP_
#define _INCLUDE_EASYDELEGATE_EXCEPTIONS_HPP_

#include <stdexcept>  // std::exception

namespace EasyDelegate
{
    /**
     *  @brief A general base exception type for all exceptions that is thrown
     *  by the EasyDelegate library.
     */
    class DelegateException : public std::exception { };

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
            virtual const char* what() const throw()
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
            virtual const char* what() const throw()
            {
                return "Attempted to perform a call against a NULL method pointer";
            }
    };
} // End NameSpace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_EXCEPTIONS_HPP_
