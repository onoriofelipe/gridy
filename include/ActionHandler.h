#ifndef __ACTION_HANDLER_H__
#define __ACTION_HANDLER_H__

#include <map>
#include <functional>
#include "Enums.h"

// Listen for input events which are emitted by some loop processor
///[]TODO: implement as queue, with processing just with get_event
///[]TODO: change generic enum to more generic events that allow payloads,
//         such as complex combat events?
class ActionHandler {
public:
   template <typename F>
   void register_action_handler(Action action, F&& handler){
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

#endif // __ACTION_HANDLER_H__
