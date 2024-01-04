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
   ActionHandler action_handler;
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
         this->position->y += 1;
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
