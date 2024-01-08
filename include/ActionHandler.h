#ifndef __ACTION_HANDLER_H__
#define __ACTION_HANDLER_H__

#include <map>
#include <functional>
#include "Enums.h"

// Listen for input events which are emitted by some loop processor
///[]TODO: implement as queue, with processing just with get_event
///[]TODO: change generic enum to more generic events that allow payloads,
//         such as complex combat events?
///[]TODO: maybe use variant for return type instead of hardcoding types handled
//         with different pipelines
template <typename R>
class ActionHandler {
public:
   template <typename F>
   void register_action_handler(Action action, F&& handler){
      action_map[action] = std::function<R(void)>(handler);
   }
   // template <typename F, typename R>
   // void register_action_handler_with_return(Action action, F&& handler){
   //    static std::map<Action, std::function<R(void)>> action_map_with_return{};
   //    action_map_with_return[action] = std::function<R(void)>(handler);
   // }
   // void on_action(Action action){
   //    auto it = action_map.find(action);
   //    if(it != action_map.end()){
   //       // call action callback
   //       (it->second)();
   //    }
   // }
   // template <typename R>
   R on_action(Action action){
      auto it = action_map.find(action);
      if(it != action_map.end()){
         // call action callback
         std::cout << "found action in ActionHandler<R>" << std::endl;
         std::cerr << "found action in ActionHandler<R>" << std::endl;
         return (it->second)();
      }
      std::cout << "Action not found in ActionHandler<R>, returning default!!!" << std::endl;
      std::cerr << "Action not found in ActionHandler<R>, returning default!!!" << std::endl;
      return R();
   }
   std::map<Action, std::function<R(void)>> action_map{};
};

template <>
class ActionHandler<void> {
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
