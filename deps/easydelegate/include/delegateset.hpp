/**
 *  @file delegateset.hpp
 *  @date 7/11/2015
 *  @version 2.0
 *  @brief Include file containing the definition for the DelegateSet class
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef _INCLUDE_EASYDELEGATE_DELEGATESET_HPP_
#define _INCLUDE_EASYDELEGATE_DELEGATESET_HPP_

#include <set>
#include <unordered_set>

#include "types.hpp"
#include "delegates.hpp"
#include "deferredcallers.hpp"

namespace EasyDelegate
{
    /**
     *  @brief A set of delegate instances that provides helper methods to invoke all
     *  contained delegates.
     *  @details The DelegateSet type can be described as a sink for specific event types.
     *  Typical usage of this behavior would involve creating a typedef of a DelegateSet type
     *  for a specific method signature which then has its own specialized typedefs to facilitate
     *  the creation of StaticDelegate and MemberDelegate types that are compatible with instances
     *  of this new specialized DelegateSet type.
     */
    template <typename returnType, typename... parameters>
    class DelegateSet : public std::set<ITypedDelegate<returnType, parameters...> *>
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
            typedef ITypedDelegate<returnType, parameters...> StoredDelegateType;

            //! Helper typedef referring to a static function pointer.
            typedef returnType(*StaticDelegateFuncPtr)(parameters...);
            //! Helper typedef referring to a member function pointer.
            template <typename classType>
            using MemberDelegateFuncPtr = returnType(classType::*)(parameters...);

            #ifndef EASYDELEGATE_NO_DEFERRED_CALLING
                /**
                 *  @brief A helper typedef to a DeferredMemberCaller that works against the specified
                 *  class and matches the signatures stored in this delegate set.
                 */
                template <typename classType>
                using DeferredMemberCallerType = DeferredMemberCaller<classType, returnType, parameters...>;

                /**
                 *  @brief A helper typedef to a DeferredStaticCaller matches the signatures stored in this
                 *  delegate set.
                 */
                typedef DeferredStaticCaller<returnType, parameters...> DeferredStaticCallerType;
            #endif

            //! Helper typedef to an std::set that is compatible with the return types of delegates stored here.
            typedef std::set<returnType> ReturnSetType;

            //! Standard destructor.
            ~DelegateSet(void)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    delete *it;
            }

            /**
             *  @brief Invoke all delegates in the set, ignoring return values.
             *  @param params All other arguments that will be used as parameters to each delegate.
             *  @throw InvalidMethodPointerException Thrown when assertions are disabled and either
             *  a stored MemberDelegate or StaticDelegate type have a NULL function to call.
             *  @throw InvalidThisPointerException Thrown when assertions are disabled and a stored
             *  MemberDelegate is attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the DelegateException cases but rather
             *  assert if assertions are enabled.
             *  @note If this throws an exception, the invocation of the set halts.
             */
            EASYDELEGATE_INLINE void invoke(parameters... params) const
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    (*it)->invoke(params...);
            }

            /**
             *  @brief Invoke all delegates in the set, storing return values in out.
             *  @param out The std::set that all return values will be sequentially written to.
             *  @param params All other arguments that will be used as parameters to each delegate.
             *  @throw InvalidMethodPointerException Thrown when assertions are disabled and either
             *  a stored MemberDelegate or StaticDelegate type have a NULL function to call.
             *  @throw InvalidThisPointerException Thrown when assertions are disabled and a stored
             *  MemberDelegate is attempting to call against a NULL this pointer.
             *  @throw std::exception Any exception can be potentially thrown by the functions each delegate calls.
             *  @note The call will not throw an exception in any of the DelegateException cases but rather
             *  assert if assertions are enabled.
             *  @note If this throws an exception, the invocation of the set halts.
             */
            EASYDELEGATE_INLINE void invoke(std::set<returnType>& out, parameters... params) const
            {
                for (auto it = this->begin(); it != this->end(); it++)
                    out.insert(out.end(), (*it)->invoke(params...));
            }

            /**
             *  @brief Pushes a delegate instance to the end of the set.
             *  @param delegateInstance The delegate instance to the pushed onto the set.
             *  @warning Ownership of the delegate will be given to the set, therefore the
             *  given delegate should not be deleted manually.
             */
            EASYDELEGATE_INLINE void push_back(StoredDelegateType* delegateInstance)
            {
                this->insert(this->end(), delegateInstance);
            }

            /**
             *  @brief Pushes a delegate instance to the end of the set.
             *  @param delegateInstance The delegate instance to the pushed onto the set.
             *  @warning Ownership of the delegate will be given to the set, therefore the
             *  given delegate should not be deleted manually.
             */
            EASYDELEGATE_INLINE void operator +=(StoredDelegateType* delegateInstance)
            {
                this->push_back(delegateInstance);
            }

            /**
             *  @brief Removes all delegates from the set that have the given class member method address
             *  for its method.
             *  @param method The class method method poiner to check against.
             *  @param deleteInstances A boolean representing whether or not all matches should be deleted when removed. The default
             *  for this parameter is true for better memory management.
             *  @param out A pointer to an std::unordered_set templated for the ITypedDelegate type that this DelegateSet contains. This set
             *  will then be populated with a list of removed delegates if deleteInstances if false. If deleteInstances is true, this list will
             *  never be populated as the removed delegates are simply deleted.
             *  @warning If deleteInstances=false, then the delegates written to out will have their ownership transferred to whatever made
             *  the call, so they must be deletated accordingly.
             */
            template <typename className>
            EASYDELEGATE_INLINE void removeDelegateByMethod(const MemberDelegateFuncPtr<className> method, const bool& deleteInstances=true, std::unordered_set<StoredDelegateType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    StoredDelegateType* current = *it;

                    if (current->callsMethod(method))
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
             *  @brief Removes all delegates from the set that have the given static method address
             *  for it's method.
             *  @param methodPointer The static method pointer to check against.
             *  @param deleteInstances A boolean representing whether or not all matches should be deleted when removed. The default
             *  for this parameter is true for better memory management.
             *  @param out A pointer to an std::unordered_set templated for the ITypedDelegate type that this DelegateSet contains. This set
             *  will then be populated with a list of removed delegates if deleteInstances is false. If deleteInstances is true, then this list
             *  will never be populated as the removed delegates are simply destroyed.
             *  @warning If deleteInstances is false and there is no out specified, you will be leaking memory if there is no other
             *  delegate sets tracking the removed delegates.
             */
            void removeDelegateByMethod(StaticDelegateFuncPtr methodPointer, const bool& deleteInstances=true, std::unordered_set<StoredDelegateType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    StoredDelegateType* current = *it;

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
             *  @param out A pointer to an std::unordered_set templated for the ITypedDelegate type that this DelegateSet contains. This set
             *  will then be populated with a list of removed delegates if deleteInstances is false. If deleteInstances is true, then this list
             *  will never be populated as the removed delegates are simply deleted.
             *  @warning If deleteInstances is false and there is no out specified, you will be leaking memory if there is no other
             *  delegate set tracking the removed delegates.
             */
            void removeDelegateByThisPointer(const void* thisPtr, const bool& deleteInstances=true, std::unordered_set<StoredDelegateType *>* out=NULL)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    StoredDelegateType* current = *it;

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
             *  @brief Removes a given delegate by its address.
             *  @param instance The delegate pointer to attempt to remove from this set.
             *  @param deleteInstance A boolean representing whether or not the target delegate should
             *  be deleted.
             *  @return A pointer to the delegate that was removed. This is NULL if none were removed or
             *  if deleteInstance is true.
             */
            StoredDelegateType* removeDelegate(StoredDelegateType* instance, const bool& deleteInstance=true)
            {
                for (auto it = this->begin(); it != this->end(); it++)
                {
                    StoredDelegateType* current = *it;

                    if (current == instance)
                    {
                        if (deleteInstance)
                            delete current;

                        this->erase(current);

                        if (deleteInstance)
                            return NULL;

                        return current;
                    }
                }

                return NULL;
            }
    };
}
#endif // _INCLUDE_EASYDELEGATE_DELEGATESET_HPP_
