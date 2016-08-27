/**
 *  @file IEngineObject.cpp
 *  @brief Source file implementing the IEngineObject interface class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/IEngineObject.hpp>

#include <game/SObjectRegistry.hpp>

namespace Kiaro
{
    namespace Game
    {
        IEngineObject::IEngineObject(void) : mID(SObjectRegistry::getPointer()->getNextObjectID())
        {
            SObjectRegistry::getPointer()->addObject(this);
        }

        const Support::String& IEngineObject::getName(void)
        {
            return mName;
        }

        IEngineObject::~IEngineObject(void)
        {
        }

        void IEngineObject::setName(const Support::String& name)
        {
            mName = name;
            SObjectRegistry::getPointer()->setNameEntry(this, name);
        }
    }
}
