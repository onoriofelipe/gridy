#ifndef __GAME_CONTEXT_H__
#define  __GAME_CONTEXT_H__

#include "Enums.h"
#include <thread>
#include <chrono>

 // so wrong to inherit from Thing 🥲
 // GameContext should handle all meta-things related to the game itself
class GameContext {
public:
   GameContext(){
      action_handler.register_action_handler(Action::Quit, [this](){
      stop();
	});
   }
   bool should_stop_loop{false};
   void stop(){
      should_stop_loop = true;
   }
   void do_game_loop(){
      // std::cout << "before game loop" << std::endl;
      while(!should_stop_loop){
         // std::cout << "inside game loop" << std::endl;
         action_emitter(Action::HandleInputs);
         // std::cout << "inputs handled" << std::endl;
         action_emitter(Action::Draw);
         // std::cout << "things drawn" << std::endl;
         std::this_thread::sleep_for(std::chrono::milliseconds(300));
      }
   }
   ActionHandler action_handler;
   action_emitter_t action_emitter;
};

#endif // __GAME_CONTEXT_H__
