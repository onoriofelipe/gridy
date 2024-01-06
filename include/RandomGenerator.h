#ifndef __RANDOM_GENERATOR_H__
#define __RANDOM_GENERATOR_H__

#include <random>
#include "Enums.h"
#include "ActionHandler.h"

///[]TODO: later change to AI controller or something similar, to emit different
//         complex behavior, maybe just asking for the behavior tree of each thing
//         to do something different or update its tick
class RandomGenerator {
public:
   RandomGenerator():
   engine{rd()},
   distribution(0, 1)
   {
      action_handler.register_action_handler(Action::RequestRandomNeighbor, [this](){
         this->position->y += 1;
      });
   }
   std::random_device rd;
   std::mt19937 engine;
   std::uniform_real_distribution<> distribution(0, 1);

   distribution(engine);
   ActionHandler action_handler;
   // action_emitter_t event_emitter;
};

#endif // __RANDOM_GENERATOR_H__