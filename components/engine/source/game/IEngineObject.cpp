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

#include <core/SObjectRegistry.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            IEngineObject::IEngineObject(void) : mID(Core::SObjectRegistry::getInstance()->getNextObjectID())
            {

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
                Core::SObjectRegistry::getInstance()->setNameEntry(this, name);
            }
        }
    }
}
