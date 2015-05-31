#include <core/SEngineInstance.hpp>

#include <stdlib.h>

#include <lua.hpp>

extern "C"
{
    int lua_engine_quit(lua_State *L)
    {
        Kiaro::Core::SEngineInstance::getPointer()->kill();
        return 1;
    }
}
