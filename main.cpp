#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>
#include <algorithm> // std::clamp
#include <numeric> // std::iota
#include <vector>
#include <thread> // this_thread sleep
#include <chrono> // milliseconds and seconds

using uchar = unsigned char;

/**
 *  AsciiScreen represents a character buffer for manipulation and printing
 *  to a character terminal with height H and width W, plus one extra column
 *  for newline characters and one extra ending null character
 */
// valid for H >= 1, W >= 1
template <uint64_t H, uint64_t W>
class AsciiScreen{
public:
   AsciiScreen(){
      for(auto i = 0; i < H*(W+1); ++i){
         buffer[i] = '~'; // why not '~'
      }
      for(auto j = 0; j < H; ++j){
         buffer[j*(W+1) + W] = '\n';
      }
      buffer[H*(W+1)-1] = '.';
      buffer[H*(W+1)] = '\0';
   }
   uint32_t cursor_line{H};
   std::array<uchar, H * (W + 1) + 1> buffer;
   ///[]TODO: rewrite api using ranges/view syntax
   uchar& pixel_ref(uint32_t column, uint32_t line){
      return buffer[line * (W+1) + column ];
   }
   uchar pixel_value(uint32_t column, uint32_t line){
      return buffer[line * (W+1) + column ];
   }
   void write_borders(uchar vertical = '|', uchar horizontal = '-', uchar corner = '+'){
      write_vertical(0, vertical);
      write_vertical(W-1, vertical);
      write_horizontal(0, horizontal);
      write_horizontal(H-1, horizontal);
      write_corners(corner);
   }
   // write a horizontal line of characters
   void write_horizontal(uint32_t line, uchar character){
      for(auto i = 0; i < W; ++i){
         buffer[line * (W+1) + i] = character;
      }
   }
   // write a vertical line of characters
   void write_vertical(uint32_t column, uchar character){
      for(auto j = 0; j < H; ++j){
         buffer[j * (W+1) + column] = character;
      }
   }
   // write a corners with characters
   void write_corners(uchar character){
      buffer[0] = character;
      buffer[W-1] = character;
      buffer[(W+1)*(H-1) + 0] = character;
      buffer[(W+1)*(H-1) + W-1] = character;
   }
   // do operation f for each pixel
   template <typename F>
   void for_pixel(const F& f){
      for(auto j = 0; j < H; ++j){
         for_line(j, f);
      }
   }
   // do operation f for each pixel in line line
   template <typename F>
   void for_line(uint32_t line, const F& f){
      for(auto i = 0; i < W; ++i){
         pixel_ref(i, line) = f(i, line);
      }
   }
   // do operation f for each pixel in column column
   template <typename F>
   void for_column(uint32_t column, const F& f){
      for(auto j = 0; j < H; ++j){
         pixel_ref(column, j) = f(column, j);
      }
   }
   // print whatever is in the buffer
   void stdout_print(){
      clear_screen();
      const char * const c_string = reinterpret_cast<const char* const>( buffer.data() );
      // std::cout << "\r\x1b[2J\x1b[H" << c_string << std::flush; // inline
      std::cout << c_string << std::flush;
   }
   void reset_cursor_position(){
      std::cout << "\x1b[H";
   }
   // use ansi witchcraft for clearing screen
   void clear_screen(){
      reset_cursor_position();
      std::cout << "\r\x1b[2J";
   }
};

/**
 * NormalizedScreen represents a screen with horizontal and vertical coordinates
 * from -1.0f to +1.0f. It uses another device for printing, initially only an
 * AsciiScreen.
 */
class NormalizedScreen{
public:
};

int main(){
   // boost::signals2::signal<void (int a)> emitter;
   // auto lambda = [](int x){ std::cout << x << std::endl; };
   // emitter.connect(lambda);
   // emitter(23);
   // emitter(14);
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
   std::cout <<   "Press ENTER to terminate."          << std::endl;
   std::cin.ignore();
}
