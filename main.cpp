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
void epilogue(){
   std::cout <<   "Press ENTER to terminate."          << std::endl;
   std::cin.ignore();
}

int main(){
   //test_normal_screen();
   // test_animation();
   test_big_negative_coefficient();
   // test_big_negative_coefficient_2();
   epilogue();
}
