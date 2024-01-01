#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "Enums.h"
#include "ActionHandler.h"
#include "GameContext.h"
#include "Game.h"
#include "AsciiScreen.h"

// responsiple for creating connections to whoever needs it
class Connector {
public:
   GameContext* game_context;
   ExternalInputHandler* input_handler;
   AsciiScreen* screen;
   Player* player;

   Connector(GameContext* game_context,
   ExternalInputHandler* input_handler,
   AsciiScreen* screen,
   Player* player):
	   game_context{game_context},
	   input_handler{input_handler},
	   screen{screen},
	   player{player}
   {
      // stablish_connections();   // done outside for now
   }
   void stablish_connections(){
      input_handler->action_emitter.connect([this](Action action){
         player->action_handler.on_action(action);
      };
      input_handler->action_emitter.connect([this](Action action){
         game_context->action_handler.on_action(action);
      };

   }

	// GameContext game_context{};
   // Player player = create_default_player();
   // ExternalInputHandler input_handler{};                                   // auto screen = NormalScreen<30, 60>{};                                   // auto screen = AsciiScreen<30, 60>{};
   // input_handler.action_emitter.connect([&](Action action){
   //    player.action_handler.on_action(action);
   // });
   // input_handler.action_emitter.connect([&](Action action){
   //    game_context.action_handler.on_action(action);
   // });
   // auto clear_screen = true;                                               // while(!game_context.should_stop_loop){                                   //   input_handler.handle_inputs();
    //   screen.reset_buffer();
    //   screen.write_borders();
    //   screen.pixel_ref(player.position.x, player.position.y) = '@';
    //   screen.stdout_print(clear_screen);
      // screen.draw_point({player.position.x, player.position.y, 0.0f});
      // screen.reset_buffer();
      ///[]TODO: do proper time accumulation so no frame skipping occurs
    //   std::this_thread::sleep_for(std::chrono::milliseconds(30));
   // }

};

#endif // __CONNECTOR_H__
