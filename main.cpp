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
   auto screen = NormalScreen<30, 30>{};
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

void test_animation(){
   auto line = Line{};
   auto screen = NormalScreen<30, 30>{};
   screen.debug = false;
   auto cycle = 3.14 * 2;
   auto increment = cycle / 30;
   auto t = cycle;
   for(auto i = 0; i < 200; ++i){
      screen.draw_line(line);
      screen.draw();
      screen.reset_buffer();
      line.p0.x = -0.5 + 0.5 * std::cos(t);
      line.p0.y = 0 + 0.5 * std::sin(t);
      line.p1.x = +0.5 + 0.5 * std::cos(t*0.7);
      line.p1.y = 0 + 0.5 * std::sin(t*0.7);
      t += increment;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }
}
void epilogue(){
   std::cout <<   "Press ENTER to terminate."          << std::endl;
   std::cin.ignore();
}

int main(){
   //test_normal_screen();
   test_animation();
   epilogue();
}
