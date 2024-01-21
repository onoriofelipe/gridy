#ifndef __ACTION_HANDLER_H__
#define __ACTION_HANDLER_H__

#include <map>
#include <functional>
#include <utility>
#include "Enums.h"

// Listen for input events which are emitted by some loop processor
///[]TODO: implement as queue, with processing just with get_event
///[]TODO: change generic enum to more generic events that allow payloads,
//         such as complex combat events?
///[]TODO: maybe use variant for return type instead of hardcoding types handled
//         with different pipelines
///[x]TODO: use perfect forwarding and variadic template
///        to replace all the redundant
///        pseudo-overloads
///        [x]it works!!! https://godbolt.org/z/5vd18rq8P
///   []TODO: think if there's a way to conveniently allow multiple types to be
///           collapsed so we don't need a single ActionHandler for each different
///           combination type; even if there' some overhead this could be acceptable;
///           and even if the types have to be hardcoded beforehand, like with std::variant

template <typename R, typename ...Args>
class ActionHandler {
public:
   template <typename F>
   void register_action_handler(Action action, F&& handler){
      action_map[action] = std::function<R(Args&&...)>(handler);
   }
   // without the template for this function:
   // not universal reference, so errors happen when trying to bind some lvalue
   // (like a local name in a lambda stack) to this Args&& which would be rvalue reference,
   // which necessarily forces the user to cast to rvalue using std::move at the call site
   // so... a template function is needed to make this a type deduction context
   // also: Blargs should equal Args
   // also: compilation time went from 2 to 18 seconds😂
   template<typename ...Blargs>
   R on_action(Action action, Blargs&&... args){
      auto it = action_map.find(action);
      if(it != action_map.end()){
         return (it->second)(std::forward<Blargs>(args)...);
      }
      ///[]TODO: decide error convention, should go anywhere it can be easily seen during debugging
      std::cout << "Action not found in ActionHandler<R,...Args>, returning default R()!!!" << std::endl;
      // std::cerr << "Action not found in ActionHandler<R,Arg>, returning default!!!" << std::endl;
      return R();
   }
   std::map<Action,std::function<R(Args...)>> action_map{};
};

// usage that should be accepted:
// ActionHandler<Position, Position, Position> action_handler;
// ActionHandler<Result, Position, Health, Kind> action_handler;
// template <typename R, typename ...Args>
// class ActionHandler {
// public:
//    template <typename F>
//    void register_action_handler(Action action, F&& handler){
//       action_map[action] = std::function<R(Args)>(handler);
//    }
//    R on_action(Action action, Args&&... args){
//       auto it = action_map.find(action);
//       if(it != action_map.end()){
//          return (it->second)(std::forward<Args>(args)...);
//       }
//       std::cout << "Action not found in ActionHandler<R,Arg>, returning default!!!" << std::endl;
//       std::cerr << "Action not found in ActionHandler<R,Arg>, returning default!!!" << std::endl;
//       return R();
//    }
//    std::map<Action,std::function<R(Args...)>> action_map{};
// };
// template <typename R>
// class ActionHandler {
// public:
//    template <typename F>
//    void register_action_handler(Action action, F&& handler){
//       action_map[action] = std::function<R(void)>(handler);
//    }
//    // template <typename F, typename R>
//    // void register_action_handler_with_return(Action action, F&& handler){
//    //    static std::map<Action, std::function<R(void)>> action_map_with_return{};
//    //    action_map_with_return[action] = std::function<R(void)>(handler);
//    // }
//    // void on_action(Action action){
//    //    auto it = action_map.find(action);
//    //    if(it != action_map.end()){
//    //       // call action callback
//    //       (it->second)();
//    //    }
//    // }
//    // template <typename R>
//    R on_action(Action action){
//       auto it = action_map.find(action);
//       if(it != action_map.end()){
//          // call action callback
//          // std::cout << "found action in ActionHandler<R>" << std::endl;
//          // std::cerr << "found action in ActionHandler<R>" << std::endl;
//          return (it->second)();
//       }
//       std::cout << "Action not found in ActionHandler<R>, returning default!!!" << std::endl;
//       std::cerr << "Action not found in ActionHandler<R>, returning default!!!" << std::endl;
//       return R();
//    }
//    std::map<Action, std::function<R(void)>> action_map{};
// };

// template <>
// class ActionHandler<void> {
// public:
//    template <typename F>
//    void register_action_handler(Action action, F&& handler){
//       action_map[action] = std::function<void(void)>(handler);
//    }

//    void on_action(Action action){
//       auto it = action_map.find(action);
//       if(it != action_map.end()){
//          // call action callback
//          (it->second)();
//       }
//    }
//    std::map<Action, std::function<void(void)>> action_map{};
// };

#endif // __ACTION_HANDLER_H__
