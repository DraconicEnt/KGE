/**
 *  @file ces.h
 *  @brief Main include file for the Component Entity System in Kiaro Game Engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_CES_H_
#define _INCLUDE_KIARO_CES_H_

#include "Entity.h"
#include "ComponentBase.h"
#include "ComponentModelRender.h"

namespace Kiaro
{
    /**
     *  @brief Namespace that contains all of the Component Entity System functions for Kiaro Game Engine.
     *  @details The <a href="http://www.gamedev.net/page/resources/_/technical/game-programming/understanding-component-entity-systems-r3013">Component Entity System</a>,
     *  or CES for short, is intended to be an improvement upon the datablock system utilized in
     *  <a href="http://en.wikipedia.org/wiki/Tribes_2">Tribes 2</a>,
     *  <a href="http://en.wikipedia.org/wiki/Torque_Game_Engine">Torque Game Engine</a>,
     *  <a href="http://en.wikipedia.org/wiki/Torque_Game_Engine_Advanced">Torque Game Engine Advanced</a> and
     *  <a href="http://en.wikipedia.org/wiki/Torque_(game_engine)#Torque_3D">Torque 3D</a>. These so-called
     *  "datablocks" were networked chunks of data that were utilized for many different tasks, most relating to
     *  the simulation itself. There were many types of datablocks, each with a highly specific task such as
     *  PlayerData which defined many physical properties related to the control of a Player in the game. In the
     *  end, this datablock system provided modders with the ability to produce total conversions, to the point
     *  that custom content could be simulated with clever arrangement of weapon images on the player and vehicles.
     *
     *  However, this system falls short in one aspect which ironically is where it excelled at during it's time.
     *  While the datablock system was flexible by far, it could have been even more flexible by allowing for the
     *  definition of objects to be made up of various components that may be added and removed at runtime to alter
     *  the behavior of a given object in predictable ways. This allows for an engine to handle both the typical
     *  cases that it was designed to handle and many others that arrise either from modding or expansion later
     *  down the line without ever touching a single line of the source code. Following Torque Game Engine's model,
     *  if you wanted a wheeled vehicle that could take flight with enough forward velocity you would have to combine
     *  the FlyingVehicle and WheeledVehicle types in the source code to get the functionality you wanted without
     *  utilizing any evil hacks that Tribes 2 modders were often forced to fall back to when the engine didn't provide
     *  any proper facilities for accomplishing a particular task.
     *
     *  In a CES paradigm, the Entity is the object that exists in your game world, not the components. An Entity
     *  however is just a list of components that are attached to it and any interested systems operate with the data
     *  provided by the components themselves. CES heavily utilizes the
     *  <a href="http://en.wikipedia.org/wiki/Composition_over_inheritance">composition over inheritance</a> paradigm
     *  to keep object definitions relatively simple.
     */
    namespace CES
    {

    } // End NameSpace CES
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_CES_H_
