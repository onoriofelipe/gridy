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
#include "GraphMap.h"

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
   std::shared_ptr<ExternalInputHandler> input_handler_;
   std::shared_ptr<Player> player_;
   std::shared_ptr<Screen> screen_;
   std::shared_ptr<RandomGenerator> random_generator_;
   std::shared_ptr<GameContext> game_context_;
   std::shared_ptr<GraphMap> graph_map_;

   Connector()
   // GameContext* game_context,
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
      game_context_ = std::make_shared<GameContext>();
      game_context_->create_some_things(); // create 4 monsters
      graph_map_ = std::make_shared<GraphMap>();
      if(graph_map_->load_graphmap_from_file("first_map.txt")){
         input_handler_ = std::make_shared<ExternalInputHandler>();
         random_generator_ = std::make_shared<RandomGenerator>();
         screen_ = std::make_shared<AsciiScreen<30,60>>();
         ///[]TODO: create the connector arguments, mostly using make_xxx(), passing
         //         the raw pointers to connector
         player_ = make_default_player();
         // auto monster_1 = make_default_monster();
         // auto monster_2 = make_default_monster();
         // Connector connector {
         //    &game_context,
         //    &input_handler,
         //    &screen,
         //    player.get()
         // };
         stablish_connections();
         game_context_->do_game_loop();
         close_connections();
      } else {
         std::cout << "loaded graphmap from file was not valid! 🥲" << std::endl;
      }
   }

   void stablish_connections(){
      // all directed communications
      connections.push_back(input_handler_->action_emitter.connect([this](Action action){
         player_->action_handler.on_action(action);
      }));
      connections.push_back(input_handler_->action_emitter.connect([this](Action action){
         game_context_->action_handler.on_action(action);
      }));
      connections.push_back(game_context_->draw_emitter.connect([this](Action action){
         screen_->action_handler.on_action(action);
      }));
      connections.push_back(game_context_->handle_inputs_emitter.connect([this](Action action){
         input_handler_->action_handler.on_action(action);
      }));
      ///[]TODO: for drawing player should be treated like any other thing actually
      connections.push_back(screen_->draw_emitter.connect([this](Screen* screen_arg){
         player_->drawing_component->draw(screen_arg);
      }));
      // many syntaxes were explored when trying to find segmentation faults;
      // no specific syntax was related to the actual bug. this is the last syntax that was tested;
      ///[]TODO: choose better syntax
      for(auto thing_it = game_context_->things.begin(); thing_it != game_context_->things.end(); ++thing_it){
         // auto p_thing = thing.get();
         connections.push_back(game_context_->event_generator.connect([=](Action action){
            (*thing_it)->action_handler.on_action(action);
         }));
         ///[]TODO: interesting point here: there can be multiple calling conventions
         connections.push_back(screen_->draw_emitter.connect([=](Screen* screen_arg){
            (*thing_it)->drawing_component->draw(screen_arg);
         }));
         connections.push_back((*thing_it)->random_direction_requester.connect([=](Action action) -> Direction {
            return random_generator_->random_direction_request_handler.on_action(action);
         }));
         connections.push_back((*thing_it)->graph_neighbor_requester.connect([=](Action action, Position position, Direction direction) -> Position {
            return graph_map_->neighbor_request_handler.on_action(action, position, direction);
            // return graph_map_->neighbor_request_handler.on_action(action, std::move(position), std::move(direction));
         }));
      }
      // for(auto thing: game_context_->things){
      //    // auto p_thing = thing.get();
      //    connections.push_back(game_context_->event_generator.connect([=](Action action){
      //       thing->action_handler.on_action(action);
      //    }));
      //    ///[]TODO: interesting point here: there can be multiple calling conventions
      //    connections.push_back(screen_->draw_emitter.connect([=](Screen* screen_arg){
      //       thing->drawing_component->draw(screen_arg);
      //    }));
      //    connections.push_back(thing->action_emitter.connect([=](Action action){
      //       random_generator_->action_handler.on_action(action);
      //    }));
      // }
      std::cerr << "connections number:" << connections.size() << std::endl;
   }
};

#endif // __CONNECTOR_H__
