/**
 *  @file IComponent.hpp
 *  @brief Include file declaring the IComponent class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

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
            // FIXME: We can automatically generate these like we do for message types
            //! An enumeration representing all of the possible component types.
            enum COMPONENT_TYPE
            {
                //! Invalid component type.
                COMPONENT_NULL = 0,

                //! The root level component.
                COMPONENT_ROOT = 1,
                //! Renderable.
                COMPONENT_RENDERABLE = 2,
                //! Physical behavior.
                COMPONENT_PHYSICAL = 3,
                //! Some logic switching.
                COMPONENT_LOGIC = 4,

                COMPONENT_PHYSICAL_LOGIC = 5,
                //! A variable of some sort.
                COMPONENT_ATTRIBUTE = 6,
                //! A physical attribute. Ie: buoyancy.
                COMPONENT_PHYSICAL_ATTRIBUTE = 7,
            };

            enum EVALUATION_STRATEGY
            {
                //! Evaluation model that simply recurses the model and calls updates.
                EVALUATION_RECURSE = 0,

                //! Evaluation model that precomputes the recursion required in the model and allows the program to simply loop over an array for evaluation.
                EVALUATION_SERIAL = 1,

                //! Evaluation model that generates a full blown update routine that is a combination of all the component update routines using JIT. This is not implemented right now.
                EVALUATION_JIT = 2,
            };

            class CModelInstance;

            /**
             *  @brief The base class for all components that can be attached to each other to eventually build a logical behavior model of sorts
             *  to enable construction of custom entities without additional clientside programming.
             */
            class IComponent
            {
                // Protected Members
                public:
                    //! All of the children stored at this level of the model.
                    Support::Vector<IComponent*> mChildren;

                    //! All children from this component downwards.
                    Support::Vector<IComponent*> mRecursiveChildren;

                    //! The root component.
                    IComponent* mRoot;

                    //! The current evaluation strategy to use.
                    EVALUATION_STRATEGY mEvaluationStrategy;

                // Private Methods
                private:
                    /**
                     *  @brief Internal method to evaluate all children components with the given entities.
                     *  @param entities The setof entities to process for.
                     *  @param children All of the children components to recurse for.
                     */
                    bool evaluateChildren(const Support::UnorderedSet<CModelInstance*>& entities, const Support::Vector<IComponent*>& children);

                    /**
                     *  @brief Internal method to return a list of all children components.
                     *  @param The output set to write the result to.
                     */
                    void getChildrenInternal(Support::Vector<IComponent*>& output);

                // Private Members
                private:
                    //! The length of the array used for serial evaluation.
                    size_t mSerialArrayLength;

                    //! The serial evaluation array. If the serial evaluation strategy is not in use, then this is simply nullptr.
                    IComponent** mSerialArray;

                // Protected Methods
                protected:
                    /**
                     *  @brief Internal validation method.
                     *  @param trail The validation trail.
                     */
                    virtual void internalValidate(Support::Vector<IComponent*> trail);

                // Public Methods
                public:
                    /**
                     *  @brief Constructor accepting a root component pointer.
                     *  @param root The pointer to the root component.
                     */
                    IComponent(IComponent* root = nullptr);

                    //! Virtual destructor.
                    virtual ~IComponent(void);

                    /**
                     *  @brief Writes the children of this component to an output vector.
                     *  @param recursive Whether or not all children should be fully recursed.
                     *  @param output The output vector to write to.
                     */
                    void getChildren(bool recursive, Support::Vector<IComponent*>& output);

                    /**
                     *  @brief Changes the strategy that will be used when evaluating the component structure and also performs
                     *  whatever precomputation and setup may be necessary to use the new strategy.
                     *  @param strategy The new evaluation strategy to use.
                     */
                    void setEvaluationStrategy(EVALUATION_STRATEGY strategy);

                    /**
                     *  @brief Returns an iterator pointing to the first child component of this component.
                     *  @return An iterator pointing to the first child component of this component.
                     */
                    Support::Vector<IComponent*>::iterator begin(void);

                    /**
                     *  @brief Returns an iterator pointing to the iterator endpoint of this component.
                     *  @return An iterator pointing to the iterator endpoint of this component.
                     */
                    Support::Vector<IComponent*>::iterator end(void);

                    /**
                     *  @brief Checks the validity of the model from this component downwards and throws an std::runtime_error if a problem
                     *  is found.
                     */
                    void validate(void);

                    /**
                     *  @brief Evaluates just the component in the model.
                     *  @param entities All entities that should be processed with this component.
                     *  @param root
                     */
                    virtual bool evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Vector<IComponent*>& root);

                    /**
                     *  @brief Evaluates the component structure from this component downwards for a specific entity.
                     *  @param entity The entity to evaluate for.
                     *  @note This is slower than batching all entities to be evaluated all into one set and processing that. Prefer
                     *  the variant of evaluate that accepts a set of entities.
                     */
                    void evaluate(CModelInstance* entity);

                    /**
                     *  @brief Evaluates a set of entities from this component downwards the hierarchy.
                     *  @param entities The set of entities to process.
                     */
                    void evaluate(const Support::UnorderedSet<CModelInstance*>& entities);

                    /**
                     *  @brief Attaches the specified component as a child component to this component. Children components should override
                     *  this method to enforce attachment/compatibility rules.
                     *  @param component The component to attach as a child.
                     */
                    virtual void attachComponent(IComponent* component);

                    virtual COMPONENT_TYPE getComponentType(void) = 0;
            };
        }
    }
}
#endif // _INCLUDE_CES_ICOMPONENT_HPP_
