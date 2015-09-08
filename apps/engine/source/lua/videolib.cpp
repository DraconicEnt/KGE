#include <core/SEngineInstance.hpp>

#include <stdlib.h>

#include <lua.hpp>

extern "C"
{
    int lua_video_setresolution(lua_State *L)
    {
        /*
        Kiaro::Core::SEngineInstance* engineCore = Kiaro::Core::SEngineInstance::getPointer();

        if (engineCore->isDedicated())
        {
            std::cerr << "VideoLib: Method 'video.setResolution' is not available for dedicated servers!" << std::endl;
            return 0;
        }
        //else if (lua_gettop(L) != 2)
       //     return luaL_error(L, "Method 'video.setResolution' takes two parameters.");
        //else if (!lua_isnumber(L, -1) || !lua_isnumber(L, -2))
       //{
        //    std::cerr << "VideoLib: Method 'video.setResolution' takes two integers." << std::endl;
        //    return 0;
        //}

        irr::IrrlichtDevice* device = engineCore->getIrrlichtDevice();

        std::cerr << "VideoLib: Method 'video.setResolution' is unimplemented. " << std::endl;
        */
        return 0;
    }

    int lua_video_setfullscreen(lua_State *L)
    {
        /*
        Kiaro::Core::SEngineInstance* engineCore = Kiaro::Core::SEngineInstance::getPointer();

        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'video.setFullscreen' is not available for dedicated servers!");
        else if (lua_gettop(L) != 1)
        {
            return luaL_error(L, "Method 'video.setResolution' takes one parameter.");
        }

        irr::IrrlichtDevice *device = engineCore->getIrrlichtDevice();

        std::cerr << "VideoLib: Method 'video.setResolution' is unimplemented. " << std::endl;
        */
        return 0;
    }

    int lua_video_takescreenshot(lua_State *L)
    {
        /*
        Kiaro::Core::SEngineInstance* engineCore = Kiaro::Core::SEngineInstance::getPointer();

        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'video.takeScreenShot' is not available for dedicated servers!");
        else if (lua_gettop(L) != 1)
            return luaL_error(L, "Method 'video.takeScreenShot' takes one parameter.");

        irr::video::IVideoDriver *video = engineCore->getIrrlichtDevice()->getVideoDriver();

        // FIXME (Robert MacGregor#9): Do I/O through PhysFS
        const Kiaro::Common::C8* fileName = luaL_checkstring(L, -1);
        video->writeImageToFile(video->createScreenShot(), fileName);

        std::cout << "VideoLib: Wrote screenshot to " << fileName << std::endl;
        */
    }
}
