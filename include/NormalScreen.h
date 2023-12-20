#ifndef __NORMAL_SCREEN_H__
#define __NORMAL_SCREEN_H__

#include "./AsciiScreen.h"
#include <algorithm> // std::clamp

using uchar = unsigned char;

uint32_t interpolate(float x, uint32_t MAX){
   // sanity
   x = std::clamp(x, -1.0f, 1.0f);
   // in range of -1 to 1, 0.5 will become 75% in range of 0% to 100%
   return ( x + 1.0f ) / (1.0f - (-1.0f)) * MAX;
}

/**
 * NormalizedScreen represents a screen with horizontal and vertical coordinates
 * from -1.0f to +1.0f. It uses another device for printing, initially only an
 * AsciiScreen.
 */
class NormalScreen {
public:
   void draw_point(const Point3& p){
      ///[]TODO: multiply by world matrix
      ///[]TODO: multiply by camera matrix
      ///[]TODO: multiply by projection matrix
      ///[]TODO: draw only after proper transformations
      m_ascii_screen.pixel_ref(  interpolate(p.x, decltype(m_ascii_screen)::Wi),
                                 interpolate(p.y, decltype(m_ascii_screen)::He)  )
                              = default_character ;
   }
   void draw(){
      m_ascii_screen.clear_screen();
      m_ascii_screen.write_borders();
      m_ascii_screen.stdout_print();
   }
   uchar default_character{'&'};
   AsciiScreen<50, 100> m_ascii_screen;
};

#endif // __NORMAL_SCREEN_H__