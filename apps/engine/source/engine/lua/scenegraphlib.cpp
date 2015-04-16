/**
 *  @file scenegraphlib.cpp
 */

#include <easylua.hpp>

#include <engine/CSceneGraph.hpp>

extern "C"
{
    int lua_scenegraphinstance_setlightingenabled(lua_State *l)
    {
        Kiaro::Engine::CSceneGraph *graph = reinterpret_cast<Kiaro::Engine::CSceneGraph *>(EasyLua::CheckUserData<Kiaro::Engine::CSceneGraph*>(l, 1, "KGE.SceneGraph"));

        return 1;
    }

    int lua_scenegraphinstance_drop(lua_State *l)
    {
        Kiaro::Engine::CSceneGraph *graph = reinterpret_cast<Kiaro::Engine::CSceneGraph *>(EasyLua::CheckUserData<Kiaro::Engine::CSceneGraph*>(l, 1, "KGE.SceneGraph"));
        delete graph;

        return 1;
    }

    int lua_scenegraphlib_create(lua_State *l)
    {
        // Create the graph
        Kiaro::Engine::CSceneGraph **result = (Kiaro::Engine::CSceneGraph **)lua_newuserdata(l, sizeof(Kiaro::Engine::CSceneGraph *));
        *result = new Kiaro::Engine::CSceneGraph();

        // Get our scene graph meta table
        luaL_getmetatable(l, "KGE.SceneGraph");

        // Now set the meta table on our user data so we can call methods against it
        lua_setmetatable(l, -2);

        return 1;
    }
}
