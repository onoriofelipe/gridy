#ifndef __ASCII_SCREEN_H__
#define __ASCII_SCREEN_H__

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

#endif // __ASCII_SCREEN_H__