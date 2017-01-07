/**
 *  @file delegateset.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Include file containing the definition for the DelegateSet class
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#if !defined(_INCLUDE_EASYDELEGATE_DELEGATESET_HPP_) && __cplusplus < 201103L
#define _INCLUDE_EASYDELEGATE_DELEGATESET_HPP_

#include <vector>
#include <functional>
//#include <unordered_set>

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
    template <typename returnType, unsigned int paramCount, typename paramOneType = int, typename paramTwoType = int, typename paramThreeType = int, typename paramFourType = int, typename paramFiveType = int>
    class DelegateSet : public std::vector<ITypedDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType> *>
    {
        public:
            typedef ITypedDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType> StoredDelegateType;

            typedef typename EasyDelegate::StaticMethodPointer<paramCount>::template Builder<paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType>::pointerType StaticDelegateFuncPtr;
            typedef StaticDelegate<returnType, paramCount, paramOneType, paramTwoType, paramThreeType, paramFourType, paramFiveType> StaticDelegateType;

            //! Standard destructor.
            ~DelegateSet(void)
            {
                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                    delete this->operator[](currentIndex);
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
            EASYDELEGATE_INLINE void invoke(...) const
            {
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

                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                    this->operator[](currentIndex)->invokeParameters(builtParameters);
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
            EASYDELEGATE_INLINE void invoke(std::vector<returnType>& out, ...) const
            {
                va_list parameters;
                va_start(parameters, paramCount);

                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                    out.push_back(this->operator[](currentIndex)->invoke(parameters));
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
             /*
            template <typename className>
            EASYDELEGATE_INLINE void removeDelegateByMethod(const MemberDelegateFuncPtr<className> method, const bool& deleteInstances=true, std::vector<StoredDelegateType *>* out=NULL)
            {
                std::vector<size_t> erasedIndices;

                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                {
                    StoredDelegateType* current = this->operator[](currentIndex);

                    if (current->callsMethod(method))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->push_back(current);

                        erasedIndices.push_back(currentIndex);
                    }
                }

                for (size_t iteration = 0; iteration < erasedIndices.size(); ++iteration)
                    this->erase(this->begin() + erasedIndices[iteration]);
            }
            */

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
            void removeDelegateByMethod(StaticDelegateFuncPtr methodPointer, const bool& deleteInstances=true, std::vector<StoredDelegateType *>* out=NULL)
            {
                std::vector<size_t> erasedIndices;

                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                {
                    StoredDelegateType* current = this->operator[](currentIndex);

                    if (current->callsMethod(methodPointer))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->push_back(current);

                        erasedIndices.push_back(currentIndex);
                    }
                }

                for (size_t iteration = 0; iteration < erasedIndices.size(); ++iteration)
                    this->erase(this->begin() + erasedIndices[iteration]);
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
             /*
            void removeDelegateByThisPointer(const void* thisPtr, const bool& deleteInstances=true, std::vector<StoredDelegateType *>* out=NULL)
            {
                std::vector<size_t> erasedIndices;

                for (size_t currentIndex = 0; currentIndex < this->size(); ++currentIndex)
                {
                    StoredDelegateType* current = this->operator[](currentIndex);

                    if (current->mIsMemberDelegate && current->hasThisPointer(thisPtr))
                    {
                        if (deleteInstances)
                            delete current;
                        else if (out)
                            out->insert(out->end(), current);

                        erasedIndices.push_back(currentIndex);
                    }
                }

                for (size_t iteration = 0; iteration < erasedIndices.size(); ++iteration)
                    this->erase(this->begin() + erasedIndices[iteration]);
            }
            */

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
                for (size_t iteration = 0; iteration < this->size(); ++iteration)
                {
                    StoredDelegateType* current = this->operator[](iteration);

                    if (current == instance)
                    {
                        if (deleteInstance)
                            delete current;

                        this->erase(this->begin() + iteration);

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
