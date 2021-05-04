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

Install bazel for your system: https://docs.bazel.build/versions/4.0.0/install.html

Currently builds have only been tested on Linux but should also work on Windows and MacOS with some prodding.

Building
--------------

Currently, the primary engine executable may be ran with:

```
bazel run //apps/main:main
```

Organization
-------------

The repository is organized into the following main three folders:

* templates - Game project starting templates. These will be moved to their own repository in the future.
* components - All components that together function as the engine.
* apps - Main binary projects. These are the main entry points to the engine, however they may be replaced in the future.

Within these directories there is a scheme of separating source and include files into their own directories. These files
generally should only contain one class and should have the same name as the class defined or implemented therein.

Class naming scheme uses a prefix scheme to indicate what a class is, refer to the table below.

* C - Ie. CBitStream. Classes with this prefix may be instanced at any time and behave like regular classes.
* I - Ie. IEntity. Classes with this prefix are an interface class and may not be instanced directly.
* S - Ie. SEngineInstance. Classes with this prefix are singletons and therefore only a single instance should exist.
