#ifndef __EVENT_GENERATOR_H__
#define __EVENT_GENERATOR_H__

#include <random>
#include "Enums.h"

///[]TODO: later change to AI controller or something similar, to emit different
//         complex behavior, maybe just asking for the behavior tree of each thing
//         to do something different or update its tick
class EventGenerator {
public:
   EventGenerator():
   engine{rd()},
   distribution(0, 1)
   {}
   std::random_device rd;
   std::mt19937 engine;
   std::uniform_real_distribution<> distribution(0, 1);

   distribution(engine);
   action_emitter_t event_emitter;
};

#endif // __EVENT_GENERATOR_H__