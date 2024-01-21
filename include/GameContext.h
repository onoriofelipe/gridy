#ifndef __GAME_CONTEXT_H__
#define  __GAME_CONTEXT_H__

#include "Enums.h"
#include "ActionHandler.h"
#include <thread>
#include <chrono>

 // so wrong to inherit from Thing 🥲
 // GameContext should handle all meta-things related to the game itself
class GameContext {
public:
   GameContext(){
      register_handlers();
      // create_some_things();
   }
   bool should_stop_loop{false};
   void stop(){
      should_stop_loop = true;
   }
   void do_game_loop(){
      // std::cout << "before game loop" << std::endl;
      auto some_counter{0};
      while(!should_stop_loop){
         // std::cout << "inside game loop" << std::endl;
         action_emitter(Action::HandleInputs);
         // std::cout << "inputs handled" << std::endl;
         action_emitter(Action::Draw);
         // std::cout << "things drawn" << std::endl;
         some_counter %= 30;
         if(some_counter == 0 ){
            action_emitter(Action::SomeTicks);
            //\\ another concept: multiple event sources, maybe for different scopes
            // std::cout << "before move random" << std::endl;
            // std::cerr << "before move random" << std::endl;
            event_generator(Action::MoveRandom);
            // std::cout << "after move random" << std::endl;
            // std::cerr << "after move random" << std::endl;
         // event_generator.generate_random_move()
         }
         ///[]TODO: do proper time accumulation so no frame skipping occurs
         std::this_thread::sleep_for(std::chrono::milliseconds(30));
      }
   }
   void create_some_things(){
      things.push_back(make_default_monster());
      things.push_back(make_default_monster());
      things.push_back(make_default_monster());
      things.push_back(make_default_monster());
   }
   void register_handlers(){
      action_handler.register_action_handler(Action::Quit, [this](){
         stop();
      });
   }
   std::vector<std::shared_ptr<Thing>> things{};
   ActionHandler<void> action_handler;
   action_emitter_t action_emitter;
   action_emitter_t event_generator;
};

#endif // __GAME_CONTEXT_H__
