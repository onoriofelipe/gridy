#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>
#include <algorithm> // std::clamp
#include <numeric> // std::iota
#include <vector>

using uchar = unsigned char;

/**
 *  AsciiScreen represents a character buffer for manipulation and printing
 *  to a character terminal with height H and width W, plus one extra column
 *  for newline characters and one extra ending null character
 */
// valid for H >= 1, W >= 1
/// []TODO: try concepts for enforcing valid ranges
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
      buffer[H*(W+1)] = '\0';
   }
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
   void for_pixel(uint32_t column, uint32_t line, const F& f){
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
   void stdout_print(){
      const char * const c_string = reinterpret_cast<const char* const>( buffer.data() );
      std::cout << c_string << std::flush;
   }
   void clear_screen(){
      for(auto j = 0; j < H; ++j){};
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
   // screen.write_horizontal(3, '\\');
   // screen.write_vertical(6, '/');
   // auto clamp = [](uint32_t x, uint32_t min, uint32_t max){ return };
   uchar min {32};
   uchar max {126};
   auto lambda = [=](uint32_t line, uint32_t column){ return std::clamp(static_cast<uchar>(line + column), min, max); };
   std::vector<uint32_t> lines(50,0);
   std::iota(lines.begin(), lines.end(), static_cast<uint32_t>(0));
   for(const auto line: lines){
      screen.for_line(line, lambda);
   }
   screen.write_borders();
   screen.stdout_print();
   std::cout << "   XXX" << std::flush;
   // std::cout << "\x1b[1A\x1b[2K" << std::flush;
   std::cout << "\x1b[1ABBBB" << std::flush;
   std::cout << "\x1b[1A\rCCCC\n" << std::flush;
   std::cout << "\x1b[1A\x1b[1A\x1b[1A" << std::flush;
   // std::cout << "\x1b[1A\x1b[2K\x1b[1A\x1b[2K" << std::flush;
}
