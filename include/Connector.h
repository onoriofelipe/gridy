#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <boost/signals2.hpp>
#include <vector>
#include "Enums.h"
#include "ActionHandler.h"
#include "GameContext.h"
#include "Game.h"
#include "AsciiScreen.h"
#include "ExternalInputHandler.h"

// responsiple for creating connections to whoever needs it
class Connector {
public:
   GameContext* game_context{nullptr};
   ExternalInputHandler* input_handler{nullptr};
   Screen* screen{nullptr};
   Player* player{nullptr};
   std::vector<boost::signals2::connection> connections{};

   Connector(GameContext* game_context,
   ExternalInputHandler* input_handler,
   Screen* screen,
   Player* player):
      game_context{game_context},
      input_handler{input_handler},
      screen{screen},
      player{player}
   {
      // stablish_connections();   // done outside for now
   }
   void close_connections(){
      for(auto& connection: connections){
         connection.disconnect();
      }
   }

   void stablish_connections(){
      connections.push_back(input_handler->action_emitter.connect([this](Action action){
         player->action_handler.on_action(action);
      })
      );
      connections.push_back(input_handler->action_emitter.connect([this](Action action){
         game_context->action_handler.on_action(action);
      })
      );
      connections.push_back(game_context->action_emitter.connect([this](Action action){
         screen->action_handler.on_action(action);
      })
      );
      connections.push_back(game_context->action_emitter.connect([this](Action action){
         input_handler->action_handler.on_action(action);
      })
      );
      connections.push_back(screen->draw_emitter.connect([this](Screen* screen_arg){
         player->drawing_component->draw(screen_arg);
      }) ///\\\here, checkpoint, next: game loop \/
      );
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
