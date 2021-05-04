Support Component
=================================

This component of the engine provides general purpose support frameworks for the game engine to operate. Generally, these classes
should be written such that they may be used outside of the context of the engine with relative ease.

The primary classes provided by this component are as follows below.

CBitStream
---------------------------------

A class used to manipulate a stream of bits. It is primarily used for serialization and deserialization of data in a raw memory buffer
for networking of data in a known packed order that is not compiler dependent.
