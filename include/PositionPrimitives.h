#ifndef __POSITION_PRIMITIVES_H__
#define __POSITION_PRIMITIVES_H__

#include <cstdio>
#include <memory>

struct Position {
   Position(int32_t x = 0, int32_t y = 0): x{x}, y{y} {}
   int32_t x;
   int32_t y;
};
struct Direction {
   Direction(int32_t x = 0, int32_t y = 0): x{x}, y{y} {}
   int32_t x;
   int32_t y;
};

Position operator+(Position p, Direction d){
   p.x += d.x;
   p.y += d.y;
   return p;
}

namespace D {
   Direction N  = Direction{ 0, 1};
   Direction NE = Direction{ 1, 1};
   Direction E  = Direction{ 1, 0};
   Direction SE = Direction{ 1,-1};
   Direction S  = Direction{ 0,-1};
   Direction SW = Direction{-1,-1};
   Direction W  = Direction{-1, 0};
   Direction NW = Direction{-1, 1};
};

std::shared_ptr<Position> make_position(int32_t x, int32_t y){
   auto p = std::make_shared<Position>(x, y);
   return p;
}

#endif // __POSITION_PRIMITIVES_H__