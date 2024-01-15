#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <functional>
#include <limits>
#include <cstdio>    // std::getchar
#include <algorithm> // clamp
#include <memory>
#include "Utils.h"
#include "Enums.h"
#include "AsciiScreen.h"
#include "ActionHandler.h"
#include "Components.h"

// too much coupling for position?
using action_position_emitter_t = boost::signals2::signal<Position(Action action)>;

///[]TODO: check if 64 bits is virtually infinite things or ir we need 128 bits
// ids: this system of identification will be used when one entity needs to
//      refer to another but they don't know anything about each other;
///[]TODO: consider the pros and cons of having a global array or vector of
//         preallocated things to make it trivial to retrieve them
struct ID {
public:
   ID(uint64_t id): id{id} {}
   static uint64_t generate_id(){
      static uint64_t counter = 0;
      ++counter;
      return counter;
   }
   uint64_t id;
};

// the eponymous game object, which has been lovingly named Thing here
class Thing {
public:
   Thing(   std::shared_ptr<Position> p,
            std::shared_ptr<Representation> r,
            const Health& health,
            const Attributes& attributes,
            std::shared_ptr<DrawingComponent> d):
            position{p},
            representation{r},
            health{health},
            attributes{attributes},
            drawing_component{d},
            id{ID::generate_id()}
   {
      // for now, initially all things are moveable as long as they receive the order
      ///[]TODO: watch out for memory explosion or fragmentation
      action_handler.register_action_handler(Action::MoveUp, [this](){
         this->position->y += 1;
      });
      action_handler.register_action_handler(Action::MoveLeft, [this](){
         this->position->x -= 1;
      });
      action_handler.register_action_handler(Action::MoveDown, [this](){
         this->position->y -= 1;
      });
      action_handler.register_action_handler(Action::MoveRight, [this](){
         this->position->x += 1;
      });
   }
   std::shared_ptr<Position> position;
   std::shared_ptr<Representation> representation;
   Health health;
   Attributes attributes;
   std::shared_ptr<DrawingComponent> drawing_component;
   ID id;
   ActionHandler<void> action_handler;
   action_emitter_t action_emitter;
   action_position_emitter_t action_position_emitter;
};

class Player: public Thing{
public:
   Player(  std::shared_ptr<Position> p,
            std::shared_ptr<Representation> r,
            const Health& health,
            const Attributes& attributes,
            std::shared_ptr<DrawingComponent> d):
         Thing(p, r, health, attributes, d)
   {}
};

std::shared_ptr<Player> make_default_player(){
   auto pos = make_position(10, 10);
   auto r = make_representation('@');
   auto d = make_drawing_component(pos, r);
   auto p = std::make_shared<Player>(pos, r, Health{}, Attributes{}, d);
   return p;
}
class Monster: public Thing {
public:
   Monster( std::shared_ptr<Position> p,
            std::shared_ptr<Representation> r,
            const Health& health,
            const Attributes& attributes,
            std::shared_ptr<DrawingComponent> d):
         Thing(p, r, health, attributes, d)
   {
      action_handler.register_action_handler(Action::MoveRandom, [this](){
         // std::cerr << "before requesting random neighbor" << std::endl;
         // Position delta_position = action_position_emitter(Action::RequestRandomNeighbor);
         if(!action_position_emitter.empty()){
            auto delta_position = action_position_emitter(Action::RequestRandomNeighbor);
            auto next_position = action_position_emitter(position? position_component? must pass data to graph map somehow);
            //\\next: define proper way to pass two world positions for the graph map, specifying positions could work but check if there is a cleaner approach,
            //        explore the possibilities
            position->x += delta_position->x;
            position->y += delta_position->y;
            // std::cerr << "thing positions updated" << std::endl;
         } else {
            std::cout << "Skipping action_position_emitter(Action::RequestRandomNeighbor) because no connections. :(" << std::endl;
         }
         // std::cout << "This action_position_emitter is associated with num_slots: " << action_position_emitter.num_slots() << std::endl;
         // std::cerr << "after requesting random neighbor" << std::endl;
      });
   }
   void draw();
};

std::shared_ptr<Monster> make_default_monster(){
   static int32_t offset{0};
   auto pos = make_position(20, 20 + offset);
   ++offset;
   auto r = make_representation('M');
   auto d = make_drawing_component(pos, r);
   auto m = std::make_shared<Monster>(pos, r, Health{}, Attributes{}, d);
   return m;
}

#endif // __GAME_H__
