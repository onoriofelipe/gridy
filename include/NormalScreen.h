#ifndef __NORMAL_SCREEN_H__
#define __NORMAL_SCREEN_H__

#include "include/AsciiScreen.h"
#include <algorithm> // std::clamp

using uchar = unsigned char;

uint32_t interpolate(float x, uint32_t MAX){
   // sanity
   x = std::clamp(x, -1.0f, 1.0f);
   // in range of -1 to 1, 0.5 will become 75% in range of 0% to 100%
   return ( x + 1.0f ) / (1.0f - (-1.0f)) * MAX;
}

class NormalScreen {
public:
   void draw_point(const Point3D& p){
      m_ascii_screen.pixel_ref(  interpolate(p.x, m_ascii_screen::W),
                                 interpolate(p.y, m_ascii_screen::H)  )
                              = default_character ;
   }
   uchar default_character{'&'};
   AsciiScreen<50, 100> m_ascii_screen;
};

#endif // __NORMAL_SCREEN_H__