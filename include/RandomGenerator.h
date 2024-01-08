#ifndef __RANDOM_GENERATOR_H__
#define __RANDOM_GENERATOR_H__

#include <random>
#include <memory> // std::shared_ptr
#include <cstdint> // int32_t
#include "Enums.h" // Action
#include "ActionHandler.h" // ActionHandler
#include "Things.h" // Position, initially

///[]TODO: later change to AI controller or something similar, to emit different
//         complex behavior, maybe just asking for the behavior tree of each thing
//         to do something different or update its tick
class RandomGenerator {
public:
   RandomGenerator():
   engine{rd()},
   float_distribution{0, 1},
   int_distribution{-1, 1}
   {
      action_handler.register_action_handler(Action::RequestRandomNeighbor, [this](/*std::shared_ptr<Position> p*/) -> Position {
         return generate_random_neighbor_delta();
      });
   }
   std::random_device rd;
   std::mt19937 engine;
   std::uniform_real_distribution<float> float_distribution;
   std::uniform_int_distribution<int32_t> int_distribution;

   // for now, return one of eight neighbors, with coords -1 or 0 or 1 for x and y
   Position generate_random_neighbor_delta(){
      int32_t x_delta{0};
      int32_t y_delta{0};
      // many more elegant ways; could map 8 neighbors to 0-7 and generate random once;
      // could define other kinds of neighbors;
      // could return 0-1 float and let the user define which neighbor has been mapped,
      // or even let the map graph decide which random direction that would be
      while(x_delta == 0 && y_delta == 0){
         x_delta = int_distribution(engine);
         y_delta = int_distribution(engine);
      }
      // std::cerr << "randomly generated delta values <x, y>: " << x_delta << ", " << y_delta << std::endl;
      // std::cout << "randomly generated delta values <x, y>: " << x_delta << ", " << y_delta << std::endl;
      return Position{x_delta, y_delta};
   }

   ActionHandler<Position> action_handler;

   ///[]TODO: neighbor API: should it be defined in terms of absolute position or in terms of deltas?
   //         deltas for now, then it becomes more generic... maybe try both approaches later, but it really
   //         feels like absolute positions requiring the absolute position as input as well makes too much coupling,
   //         but maybe not so much; one advantage to absolute positions would be that the requester
   //         does not have to implement custom logic; maybe this kind of logic should be handled by specific
   //         components inside the game entities and this becomes simply a randomness provider that provides
   //         this rng information in many flavors.
   // action_emitter_t event_emitter;
};

#endif // __RANDOM_GENERATOR_H__