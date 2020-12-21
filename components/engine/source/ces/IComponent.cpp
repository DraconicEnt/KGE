/**
 *  @file IComponent.cpp
 *  @brief Source file containing programming for the IComponent class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            void IComponent::internalValidate(Support::Vector<IComponent*> trail)
            {
                // Perform parent analysis
                for (IComponent* component: mChildren)
                {
                    for (IComponent* trailComponent: trail)
                    {
                        if (trailComponent == component)
                        {
                            throw std::runtime_error("Children components cannot occur as an eventual parent!");
                        }
                    }

                    trail.insert(trail.end(), component);
                }

                // Recurse
                for (IComponent* component: mChildren)
                {
                    component->internalValidate(trail);
                }
            }

            bool IComponent::evaluateChildren(const Support::UnorderedSet<CModelInstance*>& entities, const Support::Vector<IComponent*>& children)
            {
                // Evaluate each top level first
                for (IComponent* component: children)
                {
                    if (!component->evaluateComponent(entities, mRoot->mChildren))
                    {
                        return false;
                    }
                }

                // Then recurse
                for (IComponent* component: children)
                {
                    if (!component->evaluateChildren(entities, component->mChildren))
                    {
                        return false;
                    }
                }

                return true;
            }

            IComponent::IComponent(IComponent* root) : mRoot(root), mEvaluationStrategy(EVALUATION_RECURSE), mSerialArray(nullptr) { }

            IComponent::~IComponent(void)
            {
                for (IComponent* component: mChildren)
                {
                    delete component;
                }
            }

            void IComponent::setEvaluationStrategy(EVALUATION_STRATEGY strategy)
            {
                // Nothing to be done
                if (strategy == mEvaluationStrategy)
                {
                    return;
                }

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
                    Support::Vector<IComponent*> children;

                    this->getChildren(true, children);

                    mSerialArrayLength = children.size();
                    mSerialArray = new IComponent*[children.size()];

                    for (IComponent* component: children)
                    {
                        mSerialArray[index++] = component;
                    }
                }
            }

            void IComponent::getChildrenInternal(Support::Vector<IComponent*>& output)
            {
                for (IComponent* component: mChildren)
                {
                    output.push_back(component);
                }

                for (IComponent* component: mChildren)
                {
                    component->getChildrenInternal(output);
                }
            }

            void IComponent::getChildren(bool recursive, Support::Vector<IComponent*>& output)
            {
                output = mChildren;

                if (!recursive)
                {
                    return;
                }

                for (IComponent* component: mChildren)
                {
                    component->getChildrenInternal(output);
                }
            }

            Support::Vector<IComponent*>::iterator IComponent::begin(void)
            {
                // FIXME: Calculate this only when the model is modified
                mRecursiveChildren.clear();
                this->getChildren(true, mRecursiveChildren);

                return mRecursiveChildren.begin();
            }

            Support::Vector<IComponent*>::iterator IComponent::end(void)
            {
                return mRecursiveChildren.end();
            }

            void IComponent::validate(void)
            {
                Support::Vector<IComponent*> trail;
                this->internalValidate(trail);
            }

            bool IComponent::evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Vector<IComponent*>& root) { return true; }

            void IComponent::evaluate(CModelInstance* entity)
            {
                Support::UnorderedSet<CModelInstance*> entities;
                entities.insert(entities.end(), entity);

                this->evaluate(entities);
            }

            void IComponent::evaluate(const Support::UnorderedSet<CModelInstance*>& entities)
            {
                if (mEvaluationStrategy == EVALUATION_RECURSE && this->evaluateComponent(entities, mRoot->mChildren))
                {
                    this->evaluateChildren(entities, mChildren);
                }
                else if (mEvaluationStrategy == EVALUATION_SERIAL)
                {
                    assert(mSerialArray);

                    for (size_t iteration = 0; iteration < mSerialArrayLength; ++iteration)
                        mSerialArray[iteration]->evaluateComponent(entities, mRoot->mChildren);
                }
                else if (mEvaluationStrategy == EVALUATION_JIT)
                {
                    throw std::runtime_error("JIT evaluation type not implemented.");
                }
                else
                {
                    throw std::out_of_range("Unknown evaluation type!");
                }
            }

            void IComponent::attachComponent(IComponent* component)
            {
                component->mRoot = mRoot;
                mChildren.push_back(component);
            }
        }
    }
}
