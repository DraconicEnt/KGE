/**
 *  @file scenegraphlib.cpp
 */

#include <easylua.hpp>

#include <video/CSceneGraph.hpp>

using namespace Kiaro;

extern "C"
{
    int lua_scenegraphinstance_setlightingenabled(lua_State *l)
    {
        Video::CSceneGraph *graph = reinterpret_cast<Video::CSceneGraph *>(EasyLua::CheckUserData<Video::CSceneGraph*>(l, 1, "KGE.SceneGraph"));

        return 1;
    }

    int lua_scenegraphinstance_drop(lua_State *l)
    {
        Video::CSceneGraph *graph = reinterpret_cast<Video::CSceneGraph *>(EasyLua::CheckUserData<Video::CSceneGraph*>(l, 1, "KGE.SceneGraph"));
        delete graph;

        return 1;
    }

    int lua_scenegraphlib_create(lua_State *l)
    {
        // Create the graph
        Video::CSceneGraph **result = (Video::CSceneGraph **)lua_newuserdata(l, sizeof(Video::CSceneGraph *));
        *result = new Video::CSceneGraph();

        // Get our scene graph meta table
        luaL_getmetatable(l, "KGE.SceneGraph");

        // Now set the meta table on our user data so we can call methods against it
        lua_setmetatable(l, -2);

        return 1;
    }
}
