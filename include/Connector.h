#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <boost/signals2.hpp>
#include <vector>
#include "Enums.h"
#include "ActionHandler.h"
#include "GameContext.h"
#include "Things.h"
#include "AsciiScreen.h"
#include "ExternalInputHandler.h"
#include "RandomGenerator.h"

// []test new convention: 1 connector for each different game, connector wraps and self-contains
//   all the boilerplate logic, consider making it an hierarchy for enforcing conventions

// responsiple for creating connections to whoever needs it
class Connector {
public:
   // GameContext* game_context{nullptr};
   // ExternalInputHandler* input_handler{nullptr};
   // Screen* screen{nullptr};
   // Player* player{nullptr};
   // Player* player{nullptr};
   // EventGenerator* event_generator{nullptr};
   std::vector<boost::signals2::connection> connections{};

   Connector()
   // GameContext* game_context,
   // ExternalInputHandler* input_handler,
   // Screen* screen,
   // Player* player,
   // EventGenerator* event_generator):
   //    game_context{game_context},
   //    input_handler{input_handler},
   //    screen{screen},
   //    player{player},
   //    event_generator{event_generator}
   {}
   ///[]TODO: consider putting in destructor
   void close_connections(){
      for(auto& connection: connections){
         connection.disconnect();
      }
   }

   void generate_specifically_hardcoded_game(){
      GameContext game_context{};
      ExternalInputHandler input_handler{};
      RandomGenerator random_generator{};
      auto screen = AsciiScreen<30, 60>{};
      ///[]TODO: create the connector arguments, mostly using make_xxx(), passing
      //         the raw pointers to connector
      auto player = make_default_player();
      auto monster_1 = make_default_monster();
      auto monster_2 = make_default_monster();
      // Connector connector {
      //    &game_context,
      //    &input_handler,
      //    &screen,
      //    player.get()
      // };
      connector.stablish_connections();
      game_context.do_game_loop();
      connector.close_connections();
   }

   void stablish_connections(){
      // all directed communications
      connections.push_back(input_handler->action_emitter.connect([this](Action action){
         player->action_handler.on_action(action);
      }));
      connections.push_back(input_handler->action_emitter.connect([this](Action action){
         game_context->action_handler.on_action(action);
      }));
      connections.push_back(game_context->action_emitter.connect([this](Action action){
         screen->action_handler.on_action(action);
      }));
      connections.push_back(game_context->action_emitter.connect([this](Action action){
         input_handler->action_handler.on_action(action);
      }));
      ///[]TODO: for drawing player should be treated like any other thing actually
      connections.push_back(screen->draw_emitter.connect([this](Screen* screen_arg){
         player->drawing_component->draw(screen_arg);
      }));
      for(auto& thing: game_context->things){
         connections.push_back(game_context->event_generator.connect([this](Action action){
            thing->action_handler.on_action(action);
         }));
         ///[]TODO: interesting point here: there can be multiple calling conventions
         connections.push_back(screen->draw_emitter.connect([this](Screen* screen_arg){
            thing->drawing_component->draw(screen_arg);
         }));
         connections.push_back(thing->action_emitter.connect([this](Action action){
            random_generator->action_handler.on_action_return(action);
         }));
      }
   }
};

#endif // __CONNECTOR_H__
