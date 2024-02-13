#ifndef __FEATURE_PRIORITY_H__
#define __FEATURE_PRIORITY_H__

/*
actual priority list of priorities for real

current focus:
*[]microlist for implementing the current plan:
   []thing revamp
   []
*[]reading about behavior trees led me into a rabbit hole of game engine design
   which includes AI, tasks, events, actions, all of their respective management,
   and architecture in general. some components will be changed and integrated
   to follow new conventions. progress will be merged for now, but an overhaul
   should happen (mostly is sketched on paper but should be transferred here soon)
   []sorting in priority order the following, maybe putting as sub-tasks of the
      use cases list
   []some isolated pieces tests, maybe interfaces
   []tasks for behavior tree
   []composite nodes for behavior tree
   []a task queue
   []a task manager for executing tasks in another thread
   []a definition of what should happen each frame and the frequency for behavior
      tree ticks
   []a blackboard for the behavior tree data and communication
   []a polling station as a general gateway for getting arbitrary data from
      world entities
   []half a dozen use cases to guide the implementation of the previously mentioned
      features
      []a random move request
      []a move to direction X request
      []a pathfinding from A to B request
      []a way to use a pathfinding solution along arbitrary time durationm
         consuming partial results each time
      []thing A requests to attack thing B and the attack request goes to a
         combat handler which calculates and applies the result in the game state
      []a thing uses a spell which creates entities spread over multiple tiles,
         like fire on the floor
      []behavior tree hello world with markov-chain states
   []a combat handler
   []an entity and its component set convention
   []behavior trees
*[]think about a behavior tree
   []some tree data structure, scanned using depth first when being executed
      []each subnode is a vector ordered from left to right
      []maybe a self-balancing tree? something that can be flattened would be ideal
         to minimize indirections
         []if no dynamic behaviors are required nothing fancy such as reordering
            after construction is even needed
      []with state in a separate structure so that the behavior tree can be reused
         without copying while the per-object data is unique
         []data: current node, next node, custom behavior data
   []initial model:
      []markov chain: monster is idle, in brownian mode, or running towards or
         away from player
         []interaction with player should be tricky; likely a graph lookup would
            be ideal to find it and remain using a generic model for any entity
            without special treatment for the player; maybe this will be useful
            later as well
   []watch video example now that we thought about the problem
*[]find convention of interaction for using the asciiscreen as a camera for
   displaying just a slice of the world;
   []maybe it culls anything with position outside camera bounds;
      []maybe initial naive approach is to draw all things and if outside
         bounds then do nothing during the draw
   []maybe camera is a separate class
   []maybe things are ordered by position to allow quick culling of things to be
      drawn
   []maybe the graph can be considered a thing to be drawn, with floors and
      walls and such

[x]add vector of things to gamecontext, create some monsters, make them walk around
[x]add mersenne twister with some precooked outputs (random direction 2d, random 0-N, random 0.0-1.0, random 3x3 coordinate)
   [x]had a problem deciding how to create some sort of singleton to allow for global access for a global random generator,
      but then remembered that the signals can return values, so they could simply be connected to a single instance of a
      generator, so actions can be treated as just functions
         []someday find out the performance costs besides the connection overhead that may happen if too many components
            are instantiated in a short period of time
[sort of]connection request for runtime connections standards
[x]graphmap class for dealing with the map using graph interfaces
   [x]interface for players, monsters, things: move intention is sent to graphmap via
      event, answer is the new position
      [x]use set of constant positions as directions: N is Position{0,+1}, SW is Position{+1,-1}
   []define which tiles need individual properties; more likely we need only one piece
      of each tile type with no runtime dynamic state, and their world positions;
      [x]tiles are the vertices of graph;
      [x]connections can be defined initially by their relative position (in world coords) with no path restrictions;
      []consider adding tile properties for each individual tile; like list of things associated with it, or blood splatters
   [x]loadeable and saveable tiles from a csv file or something
      []consider serialization using cereal lib
[x]add boostbgl-backing for graphmap
   []define a sort of camera-view: if object is within camera range, then draw() writes to framebuffer, otherwise is considered culled
      [next]separate graphmap coordinates (world coordinates) from screen draw coordinates
   [x]things that operate on positions will ask the graphmap for permission
   [x]graphmap will define valid positions and valid paths between neighbors
   []graphmap may answer to shortest-path requests with valid values if distance is less than predefined range
[]try to implement a behavior tree from scratch
   []if it fails research how to do it
   [x]after proper graph movement is added?
      [x]no
[]implement A* / Djikstra
   []already implemented in bgl, just find out how to use it
[]decide how an attack can be achieved from player to monster, potentially needs the graph
   []think about graph lookup
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
      []define clearly who has which responsibility to hold data and send data
         []is it the thing? or the component held by the thing?
            []are behaviors described as collections of components or a blob in
               the thing?
   []convention for commands should be different than from requests?
   [x]whoever has the data to be operated needs to...?
      [x]for now: when sending out a request, the emitter sends the predefined information
         along with the request; the sender also needs to be connected to a service provider
         (convention: if not connected it will crash)

*/

#endif // __FEATURE_PRIORITY_H__
