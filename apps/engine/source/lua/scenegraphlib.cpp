/**
 *  @file scenegraphlib.cpp
 */

#include <easylua/easylua.hpp>

#include <video/CSceneGraph.hpp>

extern "C"
{
    int lua_scenegraphinstance_setlightingenabled(lua_State *l)
    {
        Kiaro::Video::CSceneGraph *graph = reinterpret_cast<Kiaro::Video::CSceneGraph *>(EasyLua::CheckUserData<Kiaro::Video::CSceneGraph*>(l, 1, "KGE.SceneGraph"));

        return 1;
    }

    int lua_scenegraphinstance_drop(lua_State *l)
    {
        Kiaro::Video::CSceneGraph *graph = reinterpret_cast<Kiaro::Video::CSceneGraph *>(EasyLua::CheckUserData<Kiaro::Video::CSceneGraph*>(l, 1, "KGE.SceneGraph"));
        delete graph;

        return 1;
    }

    int lua_scenegraphlib_create(lua_State *l)
    {
        // Create the graph
        Kiaro::Video::CSceneGraph **result = (Kiaro::Video::CSceneGraph **)lua_newuserdata(l, sizeof(Kiaro::Video::CSceneGraph *));
        *result = new Kiaro::Video::CSceneGraph();

        // Get our scene graph meta table
        luaL_getmetatable(l, "KGE.SceneGraph");

        // Now set the meta table on our user data so we can call methods against it
        lua_setmetatable(l, -2);

        return 1;
    }
}
