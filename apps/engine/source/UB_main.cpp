/**
 */

#ifdef ENGINE_UB

#include <lua.hpp>
//#define luai_writestring(s,l)	fwrite((s), sizeof(char), (l), stdout)

extern "C"
{
    void luai_writestring(const char* s, const size_t& length)
    {
        fwrite((s), sizeof(char), (length), stdout);
    }

    void luai_writeline(void)
    {
        luai_writestring("\n", 1);
        fflush(stdout);
    }

    #include <math.h>

    int luai_nummod(lua_State *L, int a, int b)
    {
        return (a) - l_mathop(floor)((a)/(b))*(b);
    }

    int luai_numpow(lua_State *L, int a,int b)
    {
        l_mathop(pow)(a,b);
    }
}

// Moved to avoid confusing the templates
#include "support/CBitStream.cpp"

#include "main.cpp"
#include "easylua.cpp"

#include "ces/CBehaviorModel.cpp"
#include "ces/CComponentGravity.cpp"

#include "core/Logging.cpp"
#include "core/SEngineInstance.cpp"
#include "core/SSettingsRegistry.cpp"
#include "core/tasking/CRendererTask.cpp"
#include "core/tasking/CTask.cpp"
#include "core/tasking/SAsynchronousSchedulerTask.cpp"
#include "core/tasking/SAsynchronousTaskManager.cpp"
#include "core/tasking/SSynchronousTaskManager.cpp"

#include "filesystem/CFileReader.cpp"
#include "filesystem/SResourceProvider.cpp"

#include "game/CGameAIClient.cpp"
#include "game/CGameClient.cpp"
#include "game/CScriptedGame.cpp"
#include "game/MoveManager.cpp"
#include "game/SGameServer.cpp"
#include "game/SGameWorld.cpp"
#include "game/entities/CRigidObject.cpp"
#include "game/entities/CSky.cpp"
#include "game/entities/CTerrain.cpp"
#include "game/entities/IEntity.cpp"
#include "game/entities/IRigidObject.cpp"

#include "input/SInputListener.cpp"

#include "lua/clientlib.cpp"
#include "lua/enginelib.cpp"
#include "lua/netlib.cpp"
#include "lua/scenegraphlib.cpp"
#include "lua/videolib.cpp"

#include "net/CClient.cpp"
#include "net/IServer.cpp"
#include "net/SClient.cpp"

#include "support/CCommandLineParser.cpp"
#include "support/CFileBitStream.cpp"
#include "support/CMapDivision.cpp"
#include "support/FTime.cpp"
#include "support/SEventManager.cpp"
#include "support/SSynchronousScheduler.cpp"
#include "support/TBitStream.cpp"
#include "support/TFileBitStream.cpp"

#include "video/CBulletDebugDrawer.cpp"
#include "video/CSceneGraph.cpp"
#include "video/SRenderer.cpp"

#endif
