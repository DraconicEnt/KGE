KGE
===============

The Kiaro Game Engine is a game engine that is worked on in whatever spare time [Robert MacGregor](https://github.com/Ragora]) has for a [BattleZone](http://battlezone.wikia.com/wiki/Main_Page)-esque game. It will be cross-platform and cross-compatible between Windows, Linux and OSX systems at the very least and will likely utilize Lua as the scripting language.

Ubuntu Dependencies
--------------

```
sudo apt-get install python3 cmake build-essential
```

Build Environment
--------------

Install bazel for your system: https://docs.bazel.build/versions/3.7.0/install.html

Currently builds have only been tested on Linux but should also work on Windows and MacOS with some prodding.

Building
--------------

Currently, the primary engine executable may be ran with:

```
bazel run //apps/main:main
```
