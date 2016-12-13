KGE
===

The Kiaro Game Engine is a game engine that is worked on in whatever spare time [Robert MacGregor](https://github.com/Ragora]) has for a [BattleZone](http://battlezone.wikia.com/wiki/Main_Page)-esque game. It will be cross-platform and cross-compatible between Windows, Linux and OSX systems at the very least and will likely utilize Lua as the scripting language.

Ubuntu Dependencies
===

Most dependencies can be installed via apt-get. CEGUI, however, does not appear to have a repository entry that's new
enough for our own use. (0.8.0 or higher) so it must be built from source.

```
   sudo apt-get install libenet-dev libbullet-dev libirrlicht-dev libphysfs-dev python cmake
   
   # For libCURL you may install the following packages (or an equivalent for newer versions)
   libcurl4-openssl-dev
   libcurl4-nss-dev
   libcurl4-gnutls
```

Gentoo Dependencies
===

```
   emerge net-libs/enet sci-physics/bullet dev-games/cegui dev-games/physfs dev-games/irrlicht 
```

Allegro Dependency
===

Currently, as of the time of this writing, the engine source depends on functionality only found in Allegro's work-in-progress 5.1 sources (the al_get_x_window_id method). This means you will have to build Allegro using the unstable sources [here](https://github.com/liballeg/allegro5).

Other Systems / Windows
====

If you wish to build the dependencies from source for another operating system, you may locate them at their respective pages.

  * E-Net: http://enet.bespin.org/
  * CEGUI: http://cegui.org.uk/
  * Bullet: http://bulletphysics.org/
  * PhysFS: http://icculus.org/physfs/
  * Irrlicht: http://irrlicht.sourceforge.net/
  * Allegro: http://liballeg.org/
  * libCURL: https://curl.haxx.se/libcurl/
  * CMake: https://cmake.org/
  
While a Python interpreter isn't strictly used by the engine itself, it is still used by the project generator.
  * Python: https://www.python.org/
