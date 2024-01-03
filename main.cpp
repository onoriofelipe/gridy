#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>
#include <algorithm> // std::clamp
#include <numeric> // std::iota
#include <vector>
#include <thread> // this_thread sleep
#include <chrono> // milliseconds and seconds
#include <memory> // std::shared_ptr

#include "include/Matrix.h"
// #include "include/AsciiScreen.h"
#include "include/NormalScreen.h"
#include "include/Game.h"
#include "include/Utils.h"
#include "include/PreviousTests.h"
#include "include/ExternalInputHandler.h"
#include "include/GameContext.h"
#include "include/Connector.h"

using uchar = unsigned char;

void test_connector(){
   // bool stop = false;
   GameContext game_context{};
   ExternalInputHandler input_handler{};
   auto screen = AsciiScreen<30, 60>{};
   ///[]TODO: create the connector arguments, mostly using make_xxx(), passing
   //         the raw pointers to connector
   auto player = make_default_player();
   Connector connector{
      &game_context,
      &input_handler,
      &screen,
      player.get()
   };
   connector.stablish_connections();
   game_context.do_game_loop();
   connector.close_connections();
   // GameContext game_context{};
   // Player player = create_default_player();
   // ExternalInputHandler input_handler{};
   // auto screen = NormalScreen<30, 60>{};
   // auto screen = AsciiScreen<30, 60>{};
   // input_handler.action_emitter.connect([&](Action action){
   //    player.action_handler.on_action(action);
   // });
   // input_handler.action_emitter.connect([&](Action action){
   //    game_context.action_handler.on_action(action);
   // });
   // auto clear_screen = true;
   // while(!game_context.should_stop_loop){
    //   input_handler.handle_inputs();
    //   screen.reset_buffer();
    //   screen.write_borders();
    //   screen.pixel_ref(player.position.x, player.position.y) = '@';
    //   screen.stdout_print(clear_screen);
      // screen.draw_point({player.position.x, player.position.y, 0.0f});
      // screen.reset_buffer();
      ///[]TODO: do proper time accumulation so no frame skipping occurs
    //   std::this_thread::sleep_for(std::chrono::milliseconds(30));
   // }
}

#ifdef __linux__
   struct termios global_original_termios;
#endif
// enables some form of raw mode, customized so it doesn't break the
// terminal in linux/android/termux
// reference is:
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
void setup_termios(){
   #ifdef __linux__
   // struct termios old_tio; //new_tio;
   // tcgetattr(STDIN_FILENO, &old_tio);
   // old_tio.c_lflag &= ~( ICANON | ECHO );/*& ~ECHOE  );
   // old_tio.c_lflag &=( ~ECHOE );
   // tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
   // tcsetattr(STDIN_FILENO,TCSADRAIN,&old_tio);
   // tcsetattr(STDIN_FILENO,TCSAFLUSH,&old_tio);
   //termios_p->c_lflag &= ICANON;
   tcgetattr(STDIN_FILENO, &global_original_termios);
   struct termios raw = global_original_termios;
   raw.c_iflag &= ~(BRKINT | INPCK | ICRNL | ISTRIP | IXON);
   // raw.c_oflag &= ~(OPOST); // \n becomes \n\r
   raw.c_cflag |= (CS8);
   raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
   raw.c_cc[VMIN] = 0;
   raw.c_cc[VTIME] = 1;
   tcsetattr(STDIN_FILENO,TCSANOW,&raw);
   #endif
}

///[]TODO: move setup boilerplate to its own header

void platform_independent_setup(){
   #ifdef __linux__
      setup_termios();
   #endif
}

void restore_termios(){
   #ifdef __linux__
      tcsetattr(STDIN_FILENO,TCSANOW,&global_original_termios);
   #endif
}

void epilogue(){
   #ifdef __linux__
      restore_termios();
   #endif
   std::cout <<   "\nPress ENTER to terminate."          << std::endl;
   std::cin.ignore();
}

///[]TODO: [x] hide away the ifdefs
//         [x] refactor player drawing, choose convention for drawing
//         [] next: find out if extra dynamic casts help the segmentation fault and potential slicing(?) happening
int main(){
   platform_independent_setup();
   test_connector();
   epilogue();
}