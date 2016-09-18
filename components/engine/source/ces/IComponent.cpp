/**
 *  @file IComponent.cpp
 */

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            void IComponent::internalValidate(Support::Set<IComponent*> trail)
            {
                // Perform parent analysis
                for (IComponent* component: mChildren)
                    if (trail.find(component) != trail.end())
                        throw std::runtime_error("Children components cannot occur as an eventual parent!");
                    else
                        trail.insert(trail.end(), component);

                // Recurse
                for (IComponent* component: mChildren)
                    component->internalValidate(trail);
            }

            bool IComponent::evaluateChildren(const Support::UnorderedSet<CModelInstance*>& entities, const Support::Set<IComponent*>& children)
            {
                // Evaluate each top level first
                for (IComponent* component: children)
                    if (!component->evaluateComponent(entities, mRoot->mChildren))
                        return false;

                // Then recurse
                for (IComponent* component: children)
                    if (!component->evaluateChildren(entities, component->mChildren))
                        return false;

                return true;
            }

            IComponent::IComponent(IComponent* root) : mRoot(root), mEvaluationStrategy(EVALUATION_RECURSE), mSerialArray(nullptr) { }

            IComponent::~IComponent(void)
            {
                for (IComponent* component: mChildren)
                    delete component;
            }

            void IComponent::setEvaluationStrategy(EVALUATION_STRATEGY strategy)
            {
                // Nothing to be done
                if (strategy == mEvaluationStrategy)
                    return;

                // Perform cleanup for our current evaluation strategy
                switch (mEvaluationStrategy)
                {
                    case EVALUATION_SERIAL:
                        delete[] mSerialArray;
                        mSerialArray = nullptr;

                        break;

                    case EVALUATION_RECURSE:
                    default:
                        break;
                }

                mEvaluationStrategy = strategy;

                // Perform setup for the new evaluation strategy
                if (strategy == EVALUATION_SERIAL)
                {
                    size_t index = 0;
                    Support::Set<IComponent*> children;

                    this->getChildren(true, children);

                    mSerialArrayLength = children.size();
                    mSerialArray = new IComponent*[children.size()];

                    for (IComponent* component: children)
                        mSerialArray[index++] = component;
                }
            }

            void IComponent::getChildrenInternal(Support::Set<IComponent*>& output)
            {
                // We want to return in the correct order, so we just append first
                for (IComponent* component: mChildren)
                    output.insert(output.end(), component);

                for (IComponent* component: mChildren)
                    component->getChildrenInternal(output);
            }

            void IComponent::getChildren(bool recursive, Support::Set<IComponent*>& output)
            {
                output = mChildren;

                if (!recursive)
                    return;

                for (IComponent* component: mChildren)
                    component->getChildrenInternal(output);
            }

            Support::Set<IComponent*>::iterator IComponent::begin()
            {
                // FIXME: Calculate this only when the model is modified
                this->getChildren(true, mRecursiveChildren);

                return mRecursiveChildren.begin();
            }

            Support::Set<IComponent*>::iterator IComponent::end()
            {
                return mRecursiveChildren.end();
            }

            /**
             *  @brief Checks the validity of the model from this component downwards and throws an std::runtime_error if a problem
             *  is found.
             */
            void IComponent::validate(void)
            {
                Support::Set<IComponent*> trail;
                this->internalValidate(trail);
            }

            /**
             *  @brief Evaluates just the component in the model.
             */
            bool IComponent::evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Set<IComponent*>& root) { return true; }

            void IComponent::evaluate(CModelInstance* entity)
            {
                Support::UnorderedSet<CModelInstance*> entities;
                entities.insert(entities.end(), entity);

                this->evaluate(entities);
            }

            void IComponent::evaluate(const Support::UnorderedSet<CModelInstance*>& entities)
            {
                if (mEvaluationStrategy == EVALUATION_RECURSE && this->evaluateComponent(entities, mRoot->mChildren))
                    this->evaluateChildren(entities, mChildren);
                else if (mEvaluationStrategy == EVALUATION_SERIAL)
                {
                    assert(mSerialArray);

                    for (size_t iteration = 0; iteration < mSerialArrayLength; ++iteration)
                        mSerialArray[iteration]->evaluateComponent(entities, mRoot->mChildren);
                }
                else if (mEvaluationStrategy == EVALUATION_JIT)
                    throw std::runtime_error("JIT evaluation type not implemented.");
                else
                    throw std::out_of_range("Unknown evaluation type!");
            }

            void IComponent::attachComponent(IComponent* component)
            {
                component->mRoot = mRoot;
            }
        }
    }
}
