/**
 *  @file doxymain.h
 *  @date 1/15/2017
 *  @brief Main page file for use with Doxygen. This file does absolutely nothing otherwise.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @mainpage Welcome
 *  Kiaro Game Engine is an experimental work-in-progress game engine taking some design ideology from Torque Game Engine. It is intended to
 *  take all the good design philsophy from Torque and leaving the bad behind, replacing it with better ideologies instead.
 *
 *  This is the technical documentation for all the various system internals in their current implementation state.
 *
 *  There is the following subsystems in the engine, where each subsystem has its own folder underneath of the components/ directory.
 *
 *  @link Kiaro::Engine Kiaro::Engine @endlink
 *
 *  This is the main system of the engine implementing the core capabilities that are considered to be engine itself. Here you will find actual gameplay implementation
 *  classes and specializations of the netcode programming in the Net component.
 *
 *  @link Kiaro::Net Kiaro::Net @endlink
 *
 *  This is the network programming that simply ensures that data is transported as intended from point game server to client and vice versa. It does not concern itself
 *  with serialization or any gameplay details as it simply acts as a transport mechanism for the core engine to specialize.
 *
 *  @link Kiaro::Support Kiaro::Support @endlink
 *
 *  This is all misc programming for the engine that does not fall into any one specific subsystem and are general enough to be reused elsewhere outside of the engine
 *  programming.
 *
 *  @link Kiaro::Sound Kiaro::Sound @endlink
 *
 *  This subsystem acts mostly as a wrapper around some other existing sound library to only loosely couple the engine to any given sound library, making it easier to
 *  switch sound system implementations when necessary.
 *
 */
