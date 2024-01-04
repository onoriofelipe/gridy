#ifndef __ASCII_SCREEN_H__
#define __ASCII_SCREEN_H__

#include <boost/signals2.hpp>
#include "ActionHandler.h"

using uchar = unsigned char;

class Screen;

using draw_emitter_t = boost::signals2::signal<void(Screen*)>;

class Screen {
public:
   virtual uchar& pixel_ref(uint32_t column, uint32_t line) = 0;
   virtual uchar pixel_value(uint32_t column, uint32_t line) = 0;
   virtual void write_borders(uchar vertical = '|', uchar horizontal = '-', uchar corner = '+') = 0;
   virtual void reset_buffer() = 0;
   virtual void stdout_print(bool clear_screen = true) = 0;
   virtual void draw() = 0;
   virtual void write_errors() = 0;

   draw_emitter_t draw_emitter;
   ActionHandler action_handler;
};

/**
 *  AsciiScreen represents a character buffer for manipulation and printing
 *  to a character terminal with height H and width W, plus one extra column
 *  for newline characters and one extra ending null character
 */
// valid for H >= 1, W >= 1
template <uint64_t H, uint64_t W>
class AsciiScreen: public Screen {
public:
   AsciiScreen(){
      action_handler.register_action_handler(Action::Draw, [this](){
         draw();
      });
      for(auto i = uint32_t{0}; i < H*(W+1); ++i){
         buffer[i] = '~'; // why not '~'
      }
      for(auto j = uint32_t{0}; j < H; ++j){
         buffer[j*(W+1) + W] = '\n';
      }
      buffer[H*(W+1)-1] = '.';
      buffer[H*(W+1)] = '\0';
   }
   static const uint64_t He{H};
   static const uint64_t Wi{W};
   uint32_t cursor_line{H};
   std::array<uchar, H * (W + 1) + 1> buffer;
   ///[]TODO: rewrite api using ranges/view syntax
   uchar& pixel_ref(uint32_t column, uint32_t line) override {
      line = H - 1 - line;
      return buffer[line * (W+1) + column ];
   }
   uchar pixel_value(uint32_t column, uint32_t line) override {
      return buffer[line * (W+1) + column ];
   }
   void write_borders(uchar vertical = '|', uchar horizontal = '-', uchar corner = '+') override {
      write_vertical(0, vertical);
      write_vertical(W-1, vertical);
      write_horizontal(0, horizontal);
      write_horizontal(H-1, horizontal);
      write_corners(corner);
   }
   // write a horizontal line of characters
   void write_horizontal(uint32_t line, uchar character){
      for(auto i = uint32_t{0}; i < W; ++i){
         buffer[line * (W+1) + i] = character;
      }
   }
   // write a vertical line of characters
   void write_vertical(uint32_t column, uchar character){
      for(auto j = uint32_t{0}; j < H; ++j){
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
      for(auto j = uint32_t{0}; j < H; ++j){
         for_line(j, f);
      }
   }
   void reset_buffer() override {
      for_pixel([](uint32_t, uint32_t){ return '.'; });
   }
   // do operation f for each pixel in line line
   template <typename F>
   void for_line(uint32_t line, const F& f){
      for(auto i = uint32_t{0}; i < W; ++i){
         pixel_ref(i, line) = f(i, line);
      }
   }
   // do operation f for each pixel in column column
   template <typename F>
   void for_column(uint32_t column, const F& f){
      for(auto j = uint32_t{0}; j < H; ++j){
         pixel_ref(column, j) = f(column, j);
      }
   }
   // print whatever is in the buffer
   void stdout_print(bool clear_screen = true) override {
      if(clear_screen){
         this->clear_screen();
      }
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
   void set_clear_screen_mode(bool should_clear){
      should_clear_screen = should_clear;
   }

   bool should_clear_screen{true};
   // draw whatever has been connected; their callback is supposed to
   // know what to do with the screen;
   ///[]TODO: refactor ascii screen into interface after the needed
   ///        API is more clear
   void draw() override {
      reset_buffer();
      // std::cerr << "before drawing entities" << '\n';
      draw_emitter(this);
      // std::cerr << "after drawing entities" << '\n';
      write_borders();
      stdout_print(should_clear_screen);
      // std::cerr << "done drawing" << std::endl;
   }
   ///[]TODO: write to begin of buffer or last few lines of buffer,
   ///[]TODO: consider some standardized debug display, targetting an entity
   //         in the game will display it's main data, so each relevant entity
   //         should be convertible to string, and newlines should be translated
   //         to a separate write line call, or a write range of lines call
   virtual void write_errors(){}
};

#endif // __ASCII_SCREEN_H__
