#ifndef __FEATURE_PRIORITY_H__
#define __FEATURE_PRIORITY_H__

/*
actual priority list of priorities for real

[x]add vector of things to gamecontext, create some monsters, make them walk around
[x]add mersenne twister with some precooked outputs (random direction 2d, random 0-N, random 0.0-1.0, random 3x3 coordinate)
   [x]had a problem deciding how to create some sort of singleton to allow for global access for a global random generator,
      but then remembered that the signals can return values, so they could simply be connected to a single instance of a
      generator, so actions can be treated as just functions
         []someday find out the performance costs besides the connection overhead that may happen if too many components
            are instantiated in a short period of time
[sort of]connection request for runtime connections standards
[]add boostbgl-backed graph map
[]try to implement a behavior tree from scratch
   []if it fails research how to do it
   [x]after proper graph movement is added?
      [x]no
[]implement A* / Djikstra
[]decide how an attack can be achieved from player to monster, potentially needs the graph
   []think about graph

*/

/*
emitter convention:
   []who has the information emits it?
      []no, for now use the component container for emitting, because it should
         be related to behaviors which should be handled by each entity
      []define clearly who has the 
   []convention for commands should be different than from requests?
   []whoever has the data to be operated needs to...?

*/

#endif // __FEATURE_PRIORITY_H__