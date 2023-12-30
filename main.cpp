#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>
#include <algorithm> // std::clamp
#include <numeric> // std::iota
#include <vector>
#include <thread> // this_thread sleep
#include <chrono> // milliseconds and seconds
// #include <conio.h> // getch()
#include <ncurses/ncurses.h> // getch()

#include "include/Matrix.h"
// #include "include/AsciiScreen.h"
#include "include/NormalScreen.h"
#include "include/Game.h"

using uchar = unsigned char;

void test_normal_screen(){
   auto p = Point3{0.5f, 0.5f, 0.5f};
   auto l = Line{{0.0f, 0.0f,  0.5f}, {1.0f, -1.0f, 0.5f}};
   auto l2 = Line{{-1.0f, -1.0f, 0.5f}, {1.0f, 1.0f, 0.5f}};
   auto l3 = Line{{0.0f, 0.0f, 0.5f}, {1.0f, 1.0f, 0.5f}};
   auto l4 = Line{{0.0f, 0.0f, 0.5f}, {0.9f, 0.9f, 0.5f}};
   auto l5 = Line{{0.0f, 0.0f, 0.5f}, {0.9f, 0.1f, 0.5f}};
   auto l6 = Line{{-0.5f, -0.1f, 0.5f}, {0.5f, 0.1f, 0.5f}};
   auto l7 = Line{{0.0f, 0.0f, 0.5f}, {0.5f, 0.4f, 0.5f}};
   auto l8 = Line{{0.0f, 0.0f, 0.5f}, {0.5f, -0.4f, 0.5f}};
   auto l9 = Line{{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.5f}};
   auto screen = NormalScreen<30, 60>{};
   screen.debug = false;
   screen.draw_point(p);
   // screen.draw_line(l);
   // screen.draw_line(l2);
   // screen.draw_line(l3);
   // screen.draw_line(l4);
   screen.draw_line(l5);
   screen.draw();
   screen.reset_buffer();
   screen.draw_line(l6);
   screen.draw();
   screen.reset_buffer();
   screen.draw_line(l7);
   screen.draw();
   screen.reset_buffer();
   screen.draw_line(l8);
   screen.draw();
   screen.reset_buffer();
   screen.draw_line(l9);
   screen.draw();
   screen.reset_buffer();
}

void fix_line_x_order(Line& l){
   if (l.p0.x > l.p1.x){
      ///[]TODO: specialize swap for custom POD
      // std::swap(l.p0, l.p1);
      auto temp = l.p0;
      l.p0 = l.p1;
      l.p1 = temp;
   }
}
void test_animation(){
   auto line = Line{};
   auto line2 = Line{};
   auto line3 = Line{};
   // auto screen = NormalScreen<30, 60>{};
   auto screen = NormalScreen<50, 50>{};
   screen.debug = false;
   auto cycle = 3.14 * 2;
   auto increment = cycle / 60;
   auto t = cycle;
   for(auto i = 0; i < 400; ++i){
      screen.draw_line(line);
      screen.draw_line(line2);
      screen.draw_line(line3);
      screen.draw();
      screen.reset_buffer();

      line.p0.x = -0.5 + 0.5 * std::cos(t);
      line.p0.y = 0 + 0.5 * std::sin(t);
      line.p1.x = +0.5 + 0.5 * std::cos(-t*0.7);
      line.p1.y = 0 + 0.5 * std::sin(-t*0.7);

      line2.p0.x = 0.0f;
      line2.p0.y = 0.0f;
      line2.p1.x = std::cos(t);
      line2.p1.y = std::sin(t);

      line3.p0.x = std::cos(t);
      line3.p0.y = std::sin(t);
      line3.p1.x = std::cos(t + 3.14);
      line3.p1.y = std::sin(t + 3.14);

      t += increment;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }
}
void test_big_negative_coefficient(){
   // auto line = Line{{0.0f, 0.0f, 0.5f}, {1.0f, -1.0f, 0.5f}};
   auto line0 = Line{{0.0f, 0.0f, 0.5f}, {0.75f, -1.0f, 0.5f}};
   auto line1 = Line{{0.0f, 0.0f, 0.5f}, {0.65f, -1.0f, 0.5f}};
   auto line2 = Line{{0.0f, 0.0f, 0.5f}, {0.45f, -1.0f, 0.5f}};
   auto line3 = Line{{0.0f, 0.0f, 0.5f}, {0.35f, -1.0f, 0.5f}};
   // auto line4 = Line{{0.0f, 0.0f, 0.5f}, {0.25f, -1.0f, 0.5f}};
   auto line4 = Line{{-1.0f, 1.0f, 0.5f}, {0.25f, -1.0f, 0.5f}};
   // auto line5 = Line{{0.0f, 0.0f, 0.5f}, {0.15f, -1.0f, 0.5f}};
   auto line5 = Line{{-1.0f, 1.0f, 0.5f}, {1.0f, -1.0f, 0.5f}};
   auto screen = NormalScreen<50, 150>{};
   auto clear_screen = true;
   screen.debug = false;
   // screen.draw_line(line0);
   // screen.draw_line(line1);
   // screen.draw_line(line2);
   // screen.draw_line(line3);
   for(auto i = 0; i < 600; ++i){
      // screen.draw_line(line4);
      screen.draw_line(line5);
      screen.draw(clear_screen);
      screen.reset_buffer();
      // line4.p0.x += 0.05f;
      line5.p0.x += 0.005f;
      line5.p1.x -= 0.005f;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }
}
void test_big_negative_coefficient_2(){
   // invisible
   // auto line5 = Line{{0.0f, 1.0f, 0.5f}, {0.650f, -1.0f, 0.5f}};
   auto line5 = Line{{0.0f, 1.0f, 0.5f}, {0.2f, -1.0f, 0.5f}};
   // visible
   // auto line5 = Line{{0.0f, 1.0f, 0.5f}, {0.651f, -1.0f, 0.5f}};
   auto screen = NormalScreen<50, 150>{};
   auto clear_screen = false;
   screen.debug = true;
   screen.draw_line(line5);
   screen.draw(clear_screen);
   screen.reset_buffer();
}
void test_game(){
   bool stop = false;
   InputHandler input_handler;
   input_handler.action_emitter.connect([](Action action){
      switch(action){
         case Action::MoveUp:
            std::cout << "MoveUp" << std::endl;
            break;
         case Action::MoveDown:
            std::cout << "MoveDown" << std::endl;
            break;
         case Action::MoveLeft:
            std::cout << "MoveLeft" << std::endl;
            break;
         case Action::MoveRight:
            std::cout << "MoveRight" << std::endl;
            break;
         default:
            break;
      }
   });
   while(!stop){
      input_handler.handle_inputs();
      ///[]TODO: do proper time accumulation so no frame skipping occurs
      std::this_thread::sleep_for(std::chrono::milliseconds(30));
   }
}
void test_termios_attributes(){
   
}

void epilogue(){
   // char a;
   std::cout <<   "Press ENTER to terminate."          << std::endl;
   std::cin.ignore();
   // std::cin >> a;
}

////next: input handling, ascii only
//        find reasonable way to stop input buffering and avoid need to do \n in terminal input
//        []tl;dr: non-buffered input is not easy to do without low-level apis
//        [x]apparently (n)curses is the only reasonable platform-agnostic method,
//           others would be sdl, sfml, allegro, or platform-dependent methods
//           [x]sadly, ncurses destroys my text framebuffer, I will have to surrender to using their
//              framework for everything even if I wanted only getch()
//              https://stackoverflow.com/questions/58490734/is-it-possible-to-use-curses-only-to-read-a-keypress-but-nothing-else
//              "if I put some print statement before, then getkey will clear the screen,
//              no matter if I call filter or not. I do believe that it works, because thats also what
//              I understand from the documentation, just not for me :(. I am now considering to either
//               get my key without curses or switch to using curses for all io"
//                ^ same
//           [dead]alternative: https://forums.justlinux.com/showthread.php?135548-SOLVED-Unbuffered-input-without-ncurses
//           [dead]alternative: http://www.justlinux.com/forum/showthread.php?s=&threadid=45316
//           []TODO: check which system-specific apis ncurses uses
//           []last try: https://man7.org/linux/man-pages/man3/termios.3.html
//                in non-canonical mode, the equivalent to getch:
//                MIN > 0, TIME == 0 (blocking read)
//                    read(2) blocks until MIN bytes are available, and returns
//                    up to the number of bytes requested.
//               example "raw" mode:
//               cfmakeraw() sets the terminal to something like the "raw" mode of
//               the old Version 7 terminal driver: input is available character
//               by character, echoing is disabled, and all special processing of
//               terminal input and output characters is disabled.  The terminal
//               attributes are set as follows:
//                   termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
//                                   | INLCR | IGNCR | ICRNL | IXON);
//                   termios_p->c_oflag &= ~OPOST;
//                   termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//                   termios_p->c_cflag &= ~(CSIZE | PARENB);
//                   termios_p->c_cflag |= CS8;
// struct termios {
//   tcflag_t c_iflag;    /* input specific flags (bitmask) */
//   tcflag_t c_oflag;    /* output specific flags (bitmask) */
//   tcflag_t c_cflag;    /* control flags (bitmask) */
//   tcflag_t c_lflag;    /* local flags (bitmask) */
//   cc_t     c_cc[NCCS]; /* special characters */
// https://en.wikibooks.org/wiki/Serial_Programming/termios
// There are more than 45 different flags that can be set (via tcsetattr()) or got (via tcgetattr()) with the help of the struct termios. The large number of flags, and their sometimes esoteric and pathologic meaning and behavior, is one of the reasons why serial programming under Unix can be hard. In the device configuration, one must be careful not to make a mistake.
// ^good examples in the link
// };
int main(){
   // test_normal_screen();
   // test_animation();
   // test_big_negative_coefficient();
   // test_big_negative_coefficient_2();
   // test_game();
   // std::cout << "testing getch from conio" << std::endl;
   // auto ch = getch();
   // std::cout << "testing getch from ncurses" << std::endl;
   initscr();			/* Start curses mode 		*/
   cbreak();
   noecho();
   auto ch = getch();
   std::cout << "ch = getch(): " << ch;
   // epilogue();
   endwin();
   std::cout << "zzzzzz" << std::endl;
}
