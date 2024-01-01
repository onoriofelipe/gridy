#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <functional>
#include <limits>
#include <cstdio>    // std::getchar
#include <algorithm> // clamp
#include "Utils.h"
#include "Enums.h"
#include "AsciiScreen.h"
#include "ActionHandler.h"
#include <memory>

class Thing;

// draw(DrawingComponent&, Screen&)

///[]TODO: use entities composed of components
//         (as in entity-component-system organization)
struct Position {
   uint32_t x;
   uint32_t y;
};
struct Representation {
   char c{'X'};
   char ascii(){
      return c;
   }
};
struct DrawingComponent {
   DrawingComponent(std::shared_ptr<Position>& p,
		    std::shared_ptr<Representation>& r):
	p{p}, r{r} {
      ;
   }
   void draw(AsciiScreen* screen){
      screen->get_pixel(p.x, p.y) = r.ascii();
   }
   void draw(const Position& p, const Representation& r){
      ;
   }
   // another approach being tested: some components have requirements,
   // defined by other components. ideally prepared during construction
   // but can be redefined during runtime
   std::shared_ptr<Position> p;
   std::shared_ptr<Representation> r;
};

struct Health{
   ///[]TODO: add more interesting and nonlinear information, enums
   ///[]TODO: refactor such that entities are composed of body parts with
   //         individual health information
   uint32_t max;
   uint32_t current;
};

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

struct Attributes{
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

// ExternalInputHandler maps keyboard presses in the terminal to actions;
// later it will map more inputs (mouse, gamepad, touch screen) to actions
class ExternalInputHandler {
public:
   using action_emitter_t = boost::signals2::signal<void(Action)>;
   ExternalInputHandler(){
      char_button_map = {
         {'w', Button::W},
         {'W', Button::W},
         {'a', Button::A},
         {'A', Button::A},
         {'s', Button::S},
         {'S', Button::S},
         {'d', Button::D},
         {'D', Button::D},
         {'\x1B', Button::Esc},
      };
      button_action_map = {
         {Button::W, Action::MoveUp},
         {Button::A, Action::MoveLeft},
         {Button::S, Action::MoveDown},
         {Button::D, Action::MoveRight},
         {Button::Esc, Action::Quit},
      };
      ///[]TODO: complete this using the linked documentation
      scancode_ascii_map = {
         {119, 'w'},
         {97, 'a'},
         {100, 'd'},
         {115, 's'},
         {27, '\x1B'}, // esc key
         {0, '\0'} // metacode for quitting the input loop
      };
      action_handler.register_action_handler(Action::HandleInput, [this](){
         handle_inputs();
      });
   }
   // [x]not really a scancode, but leave the abstraction later for arrows?
   char map_scancode_to_ascii(int scancode){
      char ascii{'\0'};
      auto ascii_it = scancode_ascii_map.find(scancode);
      if (ascii_it != scancode_ascii_map.end()){
         ascii = ascii_it->second;
      } else if (scancode == 0){
         // std::cout << "scancode == 0 and ascii_it == map.end()" << std::endl;
      } else {
         // std::cout << "scancode != 0 and ascii_it == map.end()" << std::endl;
         std::cout << "ascii not found: <scancode, ascii>: <" << scancode << ", " <<  static_cast<int>(ascii) << ">" << std::endl;
      }
      // std::cout << "mapping <scancode, ascii>: <" << scancode << ", " <<  static_cast<int>(ascii) << ">" << std::endl;
      return ascii;
   }
   void handle_inputs(){
      char character;
      ///[]TODO: leave input pipeline as int instead of char if input mappings
      //         are incorrectly mapped
      while ( (character = map_scancode_to_ascii(gm::getch()) ) != '\0' ){
         auto char_it = char_button_map.find(character);
         if (char_it != char_button_map.end()){
            auto button = char_it->second;
            // std::cout << "button found: " << button << std::endl;
            auto button_it = button_action_map.find(button);
            if (button_it != button_action_map.end()){
               auto action = button_it->second;
               // std::cout << "action found: " << action << std::endl;
               action_emitter(action);
            }
         }
      }
      // std::cout << "finished while in handle_inputs()" << std::endl;
   }
   action_emitter_t action_emitter;
   ActionHandler action_handler;
   std::map<Button, Action> button_action_map{};
   std::map<char, Button> char_button_map{};
   std::map<int, char> scancode_ascii_map{};
};

// Listen for input events which are emitted by some loop processor
///[]TODO: implement as queue, with processing just with get_event
///[]TODO: change generic enum to more generic events that allow payloads,
//         such as complex combat events?
class ActionHandler {
public:
   template <typename F>
   void register_action_handler(Action action, F&& handler){
      // action_map[action] = std::forward(handler);
      // action_map[action] = std::function<void(void)>(std::forward(handler));
      action_map[action] = std::function<void(void)>(handler);
   }
   void on_action(Action action){
      auto it = action_map.find(action);
      if(it != action_map.end()){
         // call action callback
         (it->second)();
      }
   }
   std::map<Action, std::function<void(void)>> action_map{};
};

///[]TODO: keep internal redundant data about all the entities located in the map;
//         when asked to move something, always update it;
//         this is the initial plan for lookups based on position required
//         for retrieving affected targets in a given area, or for interactions
//         specially for non-targetable things (hidden walls/doors/traps/
//         invisible monsters?)
//         also, it should be guaranteed to work since on spawn things will be
//         registered and have their position updated

///[]TODO: movement actions are sent to the aether;
//         the mapgraph will subscribe to those events

///[]TODO: use boost bgl for graphs?
// connected tiles list
class MapGraph {
public:
   ///[]TODO: read from file or from preset
   ///[]TODO: initially, only ground tiles and wall tiles
   // bool try_walk(from, to);
   // path_plan a_star_path(from, to);
   // ids get_ids_in_area();
};

uint32_t uniform_rand(uint32_t basic){
   // auto mt = std::mersenne_twister_thingy
   return 1*basic; // mt() * basic;
}




class Thing {
public:
   Thing(   const Position& position,
            const Health& health,
            const Attributes& attributes,
	    std::shared_ptr<Representation>& r,
	    std::shared_ptr<DrawingComponent>& ):
            position{position},
	    representation{r},
            health{health},
            attributes{attributes},
            id{ID::generate_id()}
            {}
   std::shared_ptr<Position> position;
   std::shared_ptr<Representation> representation;
   Health health;
   Attributes attributes;
   ID id;
   ActionHandler action_handler;
};

class Player: public Thing{
public:
   Player(  const Position& position,
            const Health& health,
            const Attributes& attributes,
	    std::shared_ptr<Representation>& r):
         Thing(position, health, attributes, r)
   {
      action_handler.register_action_handler(Action::MoveUp, [this](){
         this->position.y += 1;
      });
      action_handler.register_action_handler(Action::MoveLeft, [this](){
         this->position.x -= 1;
      });
      action_handler.register_action_handler(Action::MoveDown, [this](){
         this->position.y -= 1;
      });
      action_handler.register_action_handler(Action::MoveRight, [this](){
         this->position.x += 1;
      });
   }
   void draw();
   //boost::signals2::signal<?(?)> ? output_emitter;
};

Player create_default_player(){
   Player player{Position{},
                 Health{},
                 Attributes{},
                 Representation{}
                 };
   player.position.x = 10;
   player.position.y = 10;
   return player;
}
// the real difference to a monster would be probably the AI?
// so an AI component
class Monster: public Thing{
   Monster( const Position& position,
            const Health& health,
            const Attributes& attributes,
	    std::shared_ptr<Representation>& r):
         Thing(position, health, attributes, r)
         {}
   ActionHandler action_handler;
   //boost::signals2::signal<?(?)> ? output_emitter;
};

// centralizes damage formulas and possibly other combat side-effects?
class CombatMechanics {
public:
   ///[]TODO: listen for attack events and other spell events;

   uint32_t calculate_attack_damage(const Thing& agressor, const Thing& defender){
      return std::clamp(
         uniform_rand(agressor.attributes.attack)
         - uniform_rand(agressor.attributes.defense),
         static_cast<uint32_t>(0),
         std::numeric_limits<uint32_t>::max());
   }

   // for events related to world interaction, such as side-effects of spells
   // like destroying terrain, and maybe reducing health of entities; these are
   // orders, always obeyed; thus every entity listens to combat mechanics,
   // graphmap included as well
   //boost::signals2::signal<?(?)> ? output_emitter;
};

#endif // __GAME_H__
