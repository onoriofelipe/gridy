#ifndef __GAME_CONTEXT_H__
#define  __GAME_CONTEXT_H__

#include "Enums.h"

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
      while(!should_stop_loop){
         action_emitter(Action::HandleInputs);
         action_emitter(Action::Draw);
      }
   }
   ActionHandler action_handler;
   action_emitter_t action_emitter;
};

#endif // __GAME_CONTEXT_H__
