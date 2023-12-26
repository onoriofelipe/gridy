#ifndef __NORMAL_SCREEN_H__
#define __NORMAL_SCREEN_H__

#include "./AsciiScreen.h"
#include <algorithm> // std::clamp

using uchar = unsigned char;

uint32_t interpolate(float x, uint32_t MAX){
   // sanity
   x = std::clamp(x, -1.0f, 1.0f);
   // in range of -1 to 1, 0.5 will become 75% in range of 0% to 100%
   return ((( x + 1.0f ) + 0.001f ) / (1.0f - (-1.0f))) * (MAX - 1);
}

/**
 * NormalizedScreen represents a screen with horizontal and vertical coordinates
 * from -1.0f to +1.0f. It uses another device for printing, initially only an
 * AsciiScreen.
 */
template <uint64_t H, uint64_t W>
class NormalScreen {
public:
   void draw_point(const Point3& p){
      ///[]TODO: multiply by world matrix
      ///[]TODO: multiply by camera matrix
      ///[]TODO: multiply by projection matrix
      ///[]TODO: draw only after proper transformations
      draw_normalized(p.x, p.y);
   }
   void draw_line(const Line& l){
	// Bresenham
	// initially treat only case of m < 1
	// calc dx, dy
	//      dx = x2 - x1
	//      dy = y2 - y1
	// initialize x, y, p
	//      x = x1
	//      y = y1
	//      p = 2dy - dx
	// loop for x until x2
	//     print point
	//     ++x
	//     if p < 0
	//        p += 2dy
	//     else
	//        p += 2dy - 2dx
	//        ++y
   Point3 p0;
   Point3 p1;
	if(l.p0.x < l.p1.x){
      p0 = l.p0;
      p1 = l.p1;
	} else {
      p0 = l.p1;
      p1 = l.p0;
   }
	int64_t x1 = interpolate(p0.x, decltype(m_ascii_screen)::Wi);
	int64_t x2 = interpolate(p1.x, decltype(m_ascii_screen)::Wi);
	int64_t y1 = interpolate(p0.y, decltype(m_ascii_screen)::He);
	int64_t y2 = interpolate(p1.y, decltype(m_ascii_screen)::He);
	int64_t dx = x2 - x1;
	int64_t dy = y2 - y1;
	bool dy_negative = ( y1 > y2 );
	dx = std::abs(dx);
	dy = std::abs(dy);
	bool revert{false};
	if(std::abs(dx) < std::abs(dy)){
	   revert = true;
	}
	if(revert){
	   std::swap(x1, y1);
	   std::swap(x2, y2);
	   std::swap(dx, dy);
	}
	if(debug){
	std::cout << "x1, x2, y1, y2, dx, dy: "
            << x1 << " "
            << x2 << " "
            << y1 << " "
            << y2 << " "
            << dx << " "
            << dy << " "
            << std::endl;
	}
	// auto x = x1;
	auto y = y1;
	int64_t p = 2 * dy - dx;
	for (auto x = x1; x <= x2; /**/){
	   if(debug){
         std::cout << "p before iteration: " << p << std::endl;}
	   if(revert){
	      draw_directly(y, x);
	   } else {
	      draw_directly(x, y);
	   }
	   ++x;
	   if (p < 0){
	      p += 2 * dy;
	   } else {
	      p += 2 * dy - 2 * dx;
	      if( dy_negative ) {
            --y;
	      } else {
	         ++y;
	      }
	   }
	}
	if(debug){
	std::cout << "final p: " << p << std::endl;
	}
   }
   void reset_buffer(){
      m_ascii_screen.reset_buffer();
   }
   void draw_normalized(float x, float y){
      m_ascii_screen.pixel_ref(
	interpolate(x, decltype(m_ascii_screen)::Wi),
         interpolate(y, decltype(m_ascii_screen)::He)
      ) = default_character;
   }
   void draw_directly(uint32_t x, uint32_t y){
      m_ascii_screen.pixel_ref(x, y) = default_character;
   }
   void draw(bool clear_screen = true){
      // m_ascii_screen.clear_screen();
      m_ascii_screen.write_borders();
      m_ascii_screen.stdout_print(clear_screen);
   }
   uchar random_ascii_printable(){ return 'x'; }
   uchar default_character{'&'};
   AsciiScreen<H, W> m_ascii_screen;
   bool debug {true};
};

#endif // __NORMAL_SCREEN_H__
