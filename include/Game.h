#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <functional>

///[]TODO: use entities composed of components
//         (as in entity-component-system organization)
struct Position{
   uint32_t x;
   uint32_t y;
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

class Thing {
public:
   Thing(   const Position& position,
            const Health& health,
            const Attributes& attributes ):
            position{position},
            health{health},
            attributes{attributes},
            id{ID::generate_id()}
            {}
   Position position;
   Health health;
   Attributes attributes;
   ID id;
};

enum class Button {
   Up,
   Down,
   Left,
   Right,
   W,
   A,
   S,
   D,
   Q,
   E,
   F,
   R,
   G,
   T,
   C,
   V,
   Tab,
   Caps,
   _1,
   _2,
   _3,
   _4,
   _5,
};

enum class Action {
   MoveUp,
   MoveDown,
   MoveLeft,
   MoveRight,
   ToggleTargetting,
   NextTarget,
   PreviousTarget,
   TargetUp,
   TargetDown,
   TargetLeft,
   TargetRight,
   InteractHere,
   InteractWithTarget,
   NextDialogue,
   DialogueChoiceUp,
   DialogueChoiceDown,
   ToggleAutoattack,
   UseHotkey_0, // A, B, C, D
   UseHotkey_1,
   UseHotkey_2,
   UseHotkey_3,
   UseHotkey_4,
   UseHotkey_5,
   UseHotkey_6,
   UseHotkey_7,
   UseHotkey_8,
   UseHotkey_9,
   GetAttacked,
};

// Listen for input events which are emitted by some loop processor
///[]TODO: implement as queue, with processing just with get_event
///[]TODO: change generic enum to more generic events that allow payloads,
//         such as complex combat events?
class InputReceptor {
public:
   template <typename F>
   void register_action_handler(Action action, F&& handler){
      action_map[action] = std::forward(handler);
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

// centralizes damage formulas and possibly other combat side-effects?
class CombatMechanics {
public:
   ///[]TODO: listen for attack events and other spell events;

   uint32_t calculate_attack_damage(const Thing& agressor, const Thing& defender){
      return std::clamp(
         uniform_rand(agressor.attributes.attack)
         - uniform_rand(agressor.attributes.defense),
         0,
         std::numeric::MAX_LIMITS<uint32_t>);
   }

   // for events related to world interaction, such as side-effects of spells
   // like destroying terrain, and maybe reducing health of entities; these are
   // orders, always obeyed; thus every entity listens to combat mechanics,
   // graphmap included as well
   //boost::signals2::signal<?(?)> ? output_emitter;
};

class Player: public Thing{
public:
   Player(  const Position& position,
            const Health& health,
            const Attributes& attributes  ):
         Thing(position, health, attributes),
         {}
   InputReceptor input_receptor;
   //boost::signals2::signal<?(?)> ? output_emitter;
};

// the real difference to a monster would be probably the AI?
// so an AI component
class Monster: public Thing{
   Player(  const Position& position,
            const Health& health,
            const Attributes& attributes  ):
         Thing(position, health, attributes)
         {}
   InputReceptor input_receptor;
   //boost::signals2::signal<?(?)> ? output_emitter;
};

#endif // __GAME_H__