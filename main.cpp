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

void use_normal_screen(){
   auto p = Point3{0.5f, 0.5f, 0.5f};
   auto screen = NormalScreen{};
   screen.draw_point(p);
   screen.draw();
}

void epilogue(){
   std::cout <<   "Press ENTER to terminate."          << std::endl;
   std::cin.ignore();
}

int main(){
   use_normal_screen();
   epilogue();
}
