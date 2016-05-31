/**
 *  @brief IEngineObject.cpp
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
