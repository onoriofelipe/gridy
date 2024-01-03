#ifndef __COMPONENTS_H__
#define  __COMPONENTS_H__

#include <memory>
#include "AsciiScreen.h"

// draw(DrawingComponent&, Screen&)

///[]TODO: use entities composed of components
//         (as in entity-component-system organization)
///[]TODO: refactor asciiscreen in terms of coordinates centered around 0, such that they
//         can be negative?
//       []no, instead differentiate between world coordinates, which are centered around 0
//         and which can be negative, from screen coordinates; for now they are the same
//         just for the sake of simplification. this will change when world camera views
//         are implemented. for now, deal with the troubles related to subtraction with
//         unsigned integer modular arithmetic
struct Position {
   Position(int32_t x, int32_t y): x{x}, y{y} {}
   int32_t x;
   int32_t y;
};
struct Representation {
   Representation(char c): c{c} {}
   char c{'X'};
   char ascii(){
      return c;
   }
};
struct DrawingComponent {
   DrawingComponent(
      std::shared_ptr<Position> p,
      std::shared_ptr<Representation> r):
         p{p}, r{r} {}
   void draw(Screen* screen){
      screen->pixel_ref(p->x, p->y) = r->ascii();
      std::cout << "<c>: <x, y>: " << r->ascii() << ": " << p->x << ", " << p->y << std::endl;
   }
   // another approach being tested: some components have requirements,
   // defined by other components. ideally prepared during construction
   // but can be redefined during runtime
   // update: this sounds like a system in ECS model (entity-component-systems)
   std::shared_ptr<Position> p;
   std::shared_ptr<Representation> r;
};

struct Health {
   ///[]TODO: add more interesting and nonlinear information, enums
   ///[]TODO: refactor such that entities are composed of body parts with
   //         individual health information
   uint32_t max;
   uint32_t current;
};

struct Attributes {
   uint32_t attack;
   uint32_t defense;
   // for now, defense stat applies to non-physical damage as well
   // uint32_t resistance;
   // for now, no accuracy and dodge
   // uint32_t accuracy;
   // uint32_t dodge;
   // for now, no crit or other multipliers
   // uint32_t crit_chance;
   // uint32_t crit_damage;
};

std::shared_ptr<Position> make_position(int32_t x, int32_t y){
   auto p = std::make_shared<Position>(x, y);
   return p;
}
std::shared_ptr<Representation> make_representation(char c){
   auto r = std::make_shared<Representation>(c);
   return r;
}
std::shared_ptr<DrawingComponent> make_drawing_component(std::shared_ptr<Position> p,
                                                         std::shared_ptr<Representation> r){
   auto d = std::make_shared<DrawingComponent>(p, r);
   return d;
}

#endif // __COMPONENTS_H__
