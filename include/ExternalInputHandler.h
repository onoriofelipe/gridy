#ifndef __EXTERNAL_INPUT_HANDLER_H__
#define __EXTERNAL_INPUT_HANDLER_H__

#include <map>
#include "Enums.h"
#include "Utils.h"
#include "ActionHandler.h"

// ExternalInputHandler maps keyboard presses in the terminal to actions;
// later it will map more inputs (mouse, gamepad, touch screen) to actions
class ExternalInputHandler {
public:
   ExternalInputHandler(){
      char_button_map = {
         {'w', Button::W},
         {'W', Button::W},
         {'a', Button::A},
         {'A', Button::A},
         {'s', Button::S},
         {'S', Button::S},
         {'d', Button::D},
         {'D', Button::D},
         {'\x1B', Button::Esc},
      };
      button_action_map = {
         {Button::W, Action::MoveUp},
         {Button::A, Action::MoveLeft},
         {Button::S, Action::MoveDown},
         {Button::D, Action::MoveRight},
         {Button::Esc, Action::Quit},
      };
      ///[]TODO: complete this using the linked documentation
      scancode_ascii_map = {
         {119, 'w'},
         {97, 'a'},
         {100, 'd'},
         {115, 's'},
         {27, '\x1B'}, // esc key
         {0, '\0'} // metacode for quitting the input loop
      };
      action_handler.register_action_handler(Action::HandleInput, [this](){
         handle_inputs();
      });
   }
   // [x]not really a scancode, but leave the abstraction later for arrows?
   char map_scancode_to_ascii(int scancode){
      char ascii{'\0'};
      auto ascii_it = scancode_ascii_map.find(scancode);
      if (ascii_it != scancode_ascii_map.end()){
         ascii = ascii_it->second;
      } else if (scancode == 0){
         // std::cout << "scancode == 0 and ascii_it == map.end()" << std::endl;
      } else {
         // std::cout << "scancode != 0 and ascii_it == map.end()" << std::endl;
         std::cout << "ascii not found: <scancode, ascii>: <" << scancode << ", " <<  static_cast<int>(ascii) << ">" << std::endl;
      }
      // std::cout << "mapping <scancode, ascii>: <" << scancode << ", " <<  static_cast<int>(ascii) << ">" << std::endl;
      return ascii;
   }
   void handle_inputs(){
      char character;
      ///[]TODO: leave input pipeline as int instead of char if input mappings
      //         are incorrectly mapped
      while ( (character = map_scancode_to_ascii(gm::getch()) ) != '\0' ){
         auto char_it = char_button_map.find(character);
         if (char_it != char_button_map.end()){
            auto button = char_it->second;
            // std::cout << "button found: " << button << std::endl;
            auto button_it = button_action_map.find(button);
            if (button_it != button_action_map.end()){
               auto action = button_it->second;
               // std::cout << "action found: " << action << std::endl;
               action_emitter(action);
            }
         }
      }
      // std::cout << "finished while in handle_inputs()" << std::endl;
   }
   action_emitter_t action_emitter;
   ActionHandler action_handler;
   std::map<Button, Action> button_action_map{};
   std::map<char, Button> char_button_map{};
   std::map<int, char> scancode_ascii_map{};
};

#endif // __EXTERNAL_INPUT_HANDLER_H__