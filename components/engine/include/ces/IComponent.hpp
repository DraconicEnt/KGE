#ifndef _INCLUDE_CES_ICOMPONENT_HPP_
#define _INCLUDE_CES_ICOMPONENT_HPP_

#include <support/common.hpp>
#include <support/types.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            enum COMPONENT_TYPE
            {
                COMPONENT_NULL = 0,

                COMPONENT_ROOT = 1,
                COMPONENT_RENDERABLE = 2,
                COMPONENT_PHYSICAL = 3,
                COMPONENT_LOGIC = 4,
                COMPONENT_PHYSICAL_LOGIC = 5,
                COMPONENT_ATTRIBUTE = 6,
                COMPONENT_PHYSICAL_ATTRIBUTE = 7,
            };

            enum EVALUATION_STRATEGY
            {
                //! Evaluation model that simply recurses the model and calls updates.
                EVALUATION_RECURSE = 0,

                //! Evaluation model that precomputes the recursion required in the model and allows the program to simply loop over an array for evaluation.
                EVALUATION_SERIAL = 1,

                //! Evaluation model that generates a full blown update routine that is a combination of all the component update routines.
                EVALUATION_JIT = 2,
            };

            class CModelInstance;

            /**
             *  @brief
             */
            class IComponent
            {
                protected:
                    //! All of the children stored at this level of the model.
                    Support::Set<IComponent*> mChildren;

                    //! All children from this component downwards.
                    Support::Set<IComponent*> mRecursiveChildren;

                    //! The root component.
                    IComponent* mRoot;

                    EVALUATION_STRATEGY mEvaluationStrategy;

                protected:
                    virtual void internalValidate(Support::Set<IComponent*> trail);

                private:
                    bool evaluateChildren(const Support::UnorderedSet<CModelInstance*>& entities, const Support::Set<IComponent*>& children);
                    void getChildrenInternal(Support::Set<IComponent*>& output);

                    //! The length of the array used for serial evaluation.
                    size_t mSerialArrayLength;

                    //! The serial evaluation length.
                    IComponent** mSerialArray;

                public:
                    IComponent(IComponent* root = nullptr);

                    void getChildren(bool recursive, Support::Set<IComponent*>& output);

                    virtual ~IComponent(void);

                    void setEvaluationStrategy(EVALUATION_STRATEGY strategy);

                    Support::Set<IComponent*>::iterator begin();

                    Support::Set<IComponent*>::iterator end();

                    /**
                     *  @brief Checks the validity of the model from this component downwards and throws an std::runtime_error if a problem
                     *  is found.
                     */
                    void validate(void);

                    /**
                     *  @brief Evaluates just the component in the model.
                     */
                    virtual bool evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Set<IComponent*>& root);

                    void evaluate(CModelInstance* entity);

                    void evaluate(const Support::UnorderedSet<CModelInstance*>& entities);

                    virtual void attachComponent(IComponent* component);

                    virtual COMPONENT_TYPE getComponentType(void) = 0;
            };
        }
    }
}
#endif // _INCLUDE_CES_ICOMPONENT_HPP_
