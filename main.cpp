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
#include "include/Things.h"
#include "include/Utils.h"
#include "include/PreviousTests.h"
#include "include/ExternalInputHandler.h"
#include "include/GameContext.h"
#include "include/Connector.h"

using uchar = unsigned char;

void test_connector(){
   // GameContext game_context{};
   // ExternalInputHandler input_handler{};
   // auto screen = AsciiScreen<30, 60>{};
   ///[]TODO: create the connector arguments, mostly using make_xxx(), passing
   //         the raw pointers to connector
   // auto player = make_default_player();
   Connector connector{
      // &game_context,
      // &input_handler,
      // &screen,
      // player.get()
   };
   connector.generate_specifically_hardcoded_game();
   // connector.stablish_connections();
   // game_context.do_game_loop();
   // connector.close_connections();
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
   tcgetattr(STDIN_FILENO, &global_original_termios);
   struct termios raw = global_original_termios;
   raw.c_iflag &= ~(BRKINT | INPCK | ICRNL | ISTRIP | IXON);
   // raw.c_oflag &= ~(OPOST); // \n becomes \n\r, undesired currently
   raw.c_cflag |= (CS8);
   raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
   raw.c_cc[VMIN] = 0;
   raw.c_cc[VTIME] = 1;
   // options: some more buggy than others in android 8.0 to 11.0
   // TCSANOW TCSADRAIN TCSAFLUSH
   tcsetattr(STDIN_FILENO,TCSANOW,&raw);
   #endif
}

///[]TODO: move setup boilerplate to its own header
void platform_independent_setup(){
   #ifdef __linux__
      setup_termios();
   #endif
   redirect_error_stream_to_file();
   // redirect_error_stream_to_ascii_screen();
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
//         [x] find out if extra dynamic casts help the segmentation fault and potential slicing(?) happening
//         [x] cleanup and fix warnings
//         [x] error redirection in log file
//         [] future: error redirection in ascii screen as described somewhere else
//         [] future: create debug log class instead of relying on std::cerr
//         [] next: 
int main(){
   std::cout << "aaaa" << std::endl;
   // no elegant method for redirecting streams without running into raii issue yet
   std::streambuf* original_cerr_stream = std::cerr.rdbuf();
   std::ofstream error_filestream("logs.txt");
   std::cerr.rdbuf(error_filestream.rdbuf());
   platform_independent_setup();
   test_connector();
   std::cerr.rdbuf(original_cerr_stream);
   epilogue();
}