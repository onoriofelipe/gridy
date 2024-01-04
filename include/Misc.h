#ifndef __MISC_H__
#define __MISC_H__

#include "./AsciiScreen.h"
#include <algorithm> // std::clamp
#include <iostream>

using uchar = unsigned char;

void crude_animation_benchmark(){
   auto screen = AsciiScreen<50,100>{};
   uchar min {32};
   uchar max {126};
   auto lambda = [=](uint32_t line, uint32_t column){ return std::clamp(static_cast<uchar>(line + column), min, max); };
   auto generate_increasing_character_lambda = [](){
      static uchar c = 'a';
      auto lambda = [](uint32_t /*line*/, uint32_t /*column*/){ return c; };
      ++c;
      return ( lambda );
   };
   auto return_X = [](uint32_t, uint32_t){ return 'X'; };
   auto return_A = [](uint32_t, uint32_t){ return 'A'; };
   auto return_F = [](uint32_t, uint32_t){ return 'F'; };

   auto animation_duration = std::chrono::milliseconds(5000);
   auto animation_speed_in_fps = 10;
   auto total_frames = animation_duration.count() * animation_speed_in_fps / 1000;
   auto frame_duration = std::chrono::milliseconds(1000) / animation_speed_in_fps;
   auto before_program_loop = std::chrono::system_clock::now();
   for(auto i = 0; i < total_frames; ++i){
      std::this_thread::sleep_for(frame_duration);
      screen.for_pixel(generate_increasing_character_lambda());
      screen.write_borders();
      screen.stdout_print();
   }
   auto after_program_loop = std::chrono::system_clock::now();
   auto program_duration_approximately = std::chrono::duration_cast<std::chrono::milliseconds>( after_program_loop - before_program_loop );
   screen.clear_screen();
   std::cout <<   "program duration(ms): "             << program_duration_approximately.count() << std::endl;
   std::cout <<   "expected animation_duration(ms): "  << animation_duration.count()             << std::endl;
   std::cout <<   "overhead: "                         << ( static_cast<float>(program_duration_approximately.count()) / animation_duration.count() - 1 ) * 100 << "%" << std::endl;
   std::cout <<   "animation_speed_in_fps: "           << animation_speed_in_fps                 << std::endl;
   std::cout <<   "total_frames: "                     << total_frames                           << std::endl;
   std::cout <<   "frame_duration(ms): "               << frame_duration.count()                 << std::endl;
}

#endif // __MISC_H__