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
[]graphmap class for dealing with the map using graph interfaces
   []interface for players, monsters, things: move intention is sent to graphmap via
      event, answer is the new position
      []use set of constant positions as directions: N is Position{0,+1}, SW is Position{+1,-1}
   []define which tiles need individual properties; more likely we need only one piece
      of each tile type with no runtime dynamic state, and their world positions;
      []tiles are the vertices of graph;
      []connections can be defined initially by their relative position (in world coords) with no path restrictions;
      []consider adding tile properties for each individual tile; like list of things associated with it, or blood splatters
   []loadeable and saveable tiles from a csv file or something
      []consider serialization using cereal lib
[]add boostbgl-backing for graphmap
   []define a sort of camera-view: if object is within camera range, then draw() writes to framebuffer, otherwise is considered culled
      []separate graphmap coordinates (world coordinates) from screen draw coordinates
   []things that operate on positions will ask the graphmap for permission
   []graphmap will define valid positions and valid paths between neighbors
   []graphmap may answer to shortest-path requests with valid values if distance is less than predefined range
[]try to implement a behavior tree from scratch
   []if it fails research how to do it
   [x]after proper graph movement is added?
      [x]no
[]implement A* / Djikstra
[]decide how an attack can be achieved from player to monster, potentially needs the graph
   []think about graph
[]attacks make tiles dirty with directional blood splatters
[]standardize on turn duration and cooldowns

[]do bizarro world / vegan version sprites for subversion of aesthetics:
   []instead of monsters being killed, seeds are watered and bloom into cute plants
[]funny item names
   []negocim de queijo
[]enemy concepts
   []catioro da bitiura (ambush enemy) / da andradas 
   []a bitiura, mysterious entity


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
