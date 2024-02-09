#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <iostream>
#include <map>
#include <string>
#include <boost/signals2.hpp>

///[]TODO: for the problem of _runtime_ component connection (maybe should be the standard for
//         all game entities, even static ones at the level of the game):
//         request connection should be the de-facto standard API for connecting different
//         components; the providers of services can expect the thing asking for a connection
//         knows the return and has a component to fulfill the input data requirements... somehow
enum class ConnectionRequest {
   ConnectToRandomGenerator,
};

std::ostream& operator<<(std::ostream& os, const ConnectionRequest& request){
   static std::map<ConnectionRequest, std::string> request_to_string_map {
      {ConnectionRequest::ConnectToRandomGenerator,    "ConnectToRandomGenerator"},
   };
   os << request_to_string_map[request];
   return os;
}

enum class Button {
   Up,
   Down,
   Left,
   Right,
   W,
   A,
   S,
   D,
   Q,
   E,
   F,
   R,
   G,
   T,
   C,
   V,
   Tab,
   Caps,
   Esc,
   _1,
   _2,
   _3,
   _4,
   _5,
};

std::ostream& operator<<(std::ostream& os, const Button& button){
   static std::map<Button, std::string> button_to_string_map {
      {Button::Up,    "Up"},
      {Button::Down,  "Down"},
      {Button::Left,  "Left"},
      {Button::Right, "Right"},
      {Button::W,     "W"},
      {Button::A,     "A"},
      {Button::S,     "S"},
      {Button::D,     "D"},
      {Button::Q,     "Q"},
      {Button::E,     "E"},
      {Button::F,     "F"},
      {Button::R,     "R"},
      {Button::G,     "G"},
      {Button::T,     "T"},
      {Button::C,     "C"},
      {Button::V,     "V"},
      {Button::Tab,   "Tab"},
      {Button::Caps,  "Caps"},
      {Button::Esc,   "Esc"},
      {Button::_1,    "_1"},
      {Button::_2,    "_2"},
      {Button::_3,    "_3"},
      {Button::_4,    "_4"},
      {Button::_5,    "_5"},
   };
   os << button_to_string_map[button];
   return os;
}

enum class Action {
   MoveUp,
   MoveDown,
   MoveLeft,
   MoveRight,
   MoveRandom,
   ToggleTargetting,
   NextTarget,
   PreviousTarget,
   TargetUp,
   TargetDown,
   TargetLeft,
   TargetRight,
   InteractHere,
   InteractWithTarget,
   NextDialogue,
   DialogueChoiceUp,
   DialogueChoiceDown,
   ToggleAutoattack,
   UseHotkey_0, // A, B, C, D
   UseHotkey_1,
   UseHotkey_2,
   UseHotkey_3,
   UseHotkey_4,
   UseHotkey_5,
   UseHotkey_6,
   UseHotkey_7,
   UseHotkey_8,
   UseHotkey_9,
   GetAttacked,
   Quit,
   HandleInputs,
   Draw,
   RequestRandomNeighbor, // assume 8 neighbors for each cell
   RequestConnectedNeighbor,
   RequestRandomFloat,    // assume range 0.0f - 1.0f
   RequestRandomBool,     // consider using float instead? for different ranges, or also an argument somehow
   SomeTicks,
};
using action_emitter_t = boost::signals2::signal<void(Action action)>;

std::ostream& operator<<(std::ostream& os, const Action& action){
   static std::map<Action, std::string> action_to_string_map {
      {Action::MoveUp,             "MoveUp"},
      {Action::MoveDown,           "MoveDown"},
      {Action::MoveLeft,           "MoveLeft"},
      {Action::MoveRight,          "MoveRight"},
      {Action::MoveRandom,         "MoveRandom"},
      {Action::ToggleTargetting,   "ToggleTargetting"},
      {Action::NextTarget,         "NextTarget"},
      {Action::PreviousTarget,     "PreviousTarget"},
      {Action::TargetUp,           "TargetUp"},
      {Action::TargetDown,         "TargetDown"},
      {Action::TargetLeft,         "TargetLeft"},
      {Action::TargetRight,        "TargetRight"},
      {Action::InteractHere,       "InteractHere"},
      {Action::InteractWithTarget, "InteractWithTarget"},
      {Action::NextDialogue,       "NextDialogue"},
      {Action::DialogueChoiceUp,   "DialogueChoiceUp"},
      {Action::DialogueChoiceDown, "DialogueChoiceDown"},
      {Action::ToggleAutoattack,   "ToggleAutoattack"},
      {Action::UseHotkey_0,        "UseHotkey_0"},
      {Action::UseHotkey_1,        "UseHotkey_1"},
      {Action::UseHotkey_2,        "UseHotkey_2"},
      {Action::UseHotkey_3,        "UseHotkey_3"},
      {Action::UseHotkey_4,        "UseHotkey_4"},
      {Action::UseHotkey_5,        "UseHotkey_5"},
      {Action::UseHotkey_6,        "UseHotkey_6"},
      {Action::UseHotkey_7,        "UseHotkey_7"},
      {Action::UseHotkey_8,        "UseHotkey_8"},
      {Action::UseHotkey_9,        "UseHotkey_9"},
      {Action::GetAttacked,        "GetAttacked"},
      {Action::Quit,               "Quit"},
      {Action::HandleInputs,          "HandleInputs"},
      {Action::Draw,                  "Draw"},
      {Action::RequestRandomNeighbor, "RequestRandomNeighbor"},
      {Action::RequestConnectedNeighbor, "RequestConnectedNeighbor"},
      {Action::RequestRandomFloat,    "RequestRandomFloat"},
      {Action::RequestRandomBool,     "RequestRandomBool"},
      {Action::SomeTicks,             "SomeTicks"},
   };
   os << action_to_string_map[action];
   return os;
}


#endif // __ENUMS_H__
