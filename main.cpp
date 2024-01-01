#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>
#include <algorithm> // std::clamp
#include <numeric> // std::iota
#include <vector>
#include <thread> // this_thread sleep
#include <chrono> // milliseconds and seconds

#include "include/Matrix.h"
// #include "include/AsciiScreen.h"
#include "include/NormalScreen.h"
#include "include/Game.h"
#include "include/Utils.h"
#include "include/PreviousTests.h"

using uchar = unsigned char;
/*
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
void test_input_handling(){
   bool stop = false;
   ExternalInputHandler input_handler;
   input_handler.action_emitter.connect([&](Action action){
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
         case Action::Quit:
            std::cout << "Quit" << std::endl;
            stop = true;
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
void test_player_drawing(){
   // bool stop = false;
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
     //  screen.reset_buffer();
    //   screen.write_borders();
    //   screen.pixel_ref(player.position.x, player.position.y) = '@';
    //   screen.stdout_print(clear_screen);
    //   // screen.draw_point({player.position.x, player.position.y, 0.0f});
      // screen.reset_buffer();
      ///[]TODO: do proper time accumulation so no frame skipping occurs
    //   std::this_thread::sleep_for(std::chrono::milliseconds(30));
   // }
}
// drawable things should be registerable during runtime since things
// will be generated during runtime
// screen will be connected to the same callback
// draw_emitter.
void test_termios_attributes(){
auto ch = getchar();
   std::cout << "ch = getch(): " << ch;
}
*/
void test_connector(){
   // bool stop = false;
   GameContext game_context{};
   ExternalInputHandler input_handler{};
   auto screen = AsciiScreen<30, 60>{};
   Connector connector{
      &game_context,
      &input_handler,
      &screen,
      &player
   };
   connector.stablish_connections();
   gamecontext.do_game_loop();
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

void test_conio_getch(){

}

struct termios global_original_termios;

// enables some form of raw mode, customized so it doesn't break the
// terminal in linux/android/termux
// reference is:
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
void setup_termios(){
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
}

void restore_termios(){
   tcsetattr(STDIN_FILENO,TCSANOW,&global_original_termios);
}
void epilogue(){
   // char a;
#ifdef __linux__
   restore_termios();
#endif
   std::cout <<   "\nPress ENTER to terminate."          << std::endl;
   std::cin.ignore();
   // std::cin >> a;
}

///[]TODO: hide away the ifdefs
// next: refactor player drawing, choose convention for drawing
int main(){
#ifdef __linux__
   setup_termios();
#endif
   // test_normal_screen();
   // test_animation();
   // test_big_negative_coefficient();
   // test_big_negative_coefficient_2();
   // test_input_handling();
   // test_termios_attributes();
   // test_conio_getch();
   // test_player_drawing();
   test_connector();
   epilogue();
}

// 
