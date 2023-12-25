#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <array>
#include <iostream>
#include <cstdint>
#include <cmath>

using uchar = unsigned char;

class Vector3;

namespace m {

float abs(const Vector3& a);
Vector3 dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);
Vector3 normal(const Vector3& v);

} // end namespace m

class Vector2{};
class Vector3{
public:
   float x{0.0f};
   float y{0.0f};
   float z{0.0f};
   ///[]TODO: do cached length
   Vector3 dot(const Vector3 b) const{
      return m::dot(*this, b);
   }
   Vector3 normal() const{
      return m::normal(*this);
   }

   float operator()(uint32_t i) const {
      switch(i){
         case 0:
            return x;
         case 1:
            return y;
         case 2:
            return z;
         default:
            return x;
      }
   }
   float& operator[](uint32_t i) {
      switch(i){
         case 0:
            return x;
         case 1:
            return y;
         case 2:
            return z;
         default:
            return x;
      }
   }
};
class Point3{
public:
   float x{0.0f};
   float y{0.0f};
   float z{0.0f};
   float operator()(uint32_t i) const {
      switch(i){
         case 0:
            return x;
         case 1:
            return y;
         case 2:
            return z;
         default:
            return x;
      }
   }
   float& operator[](uint32_t i) {
      switch(i){
         case 0:
            return x;
         case 1:
            return y;
         case 2:
            return z;
         default:
            return x;
      }
   }
};
class Line {
public:
   //Line(float x0, float y0, float x1, float y1):
   //	p0{x0, y0, 0.f}, p1{x1, y1, 0.f} {}
   Line(): p0{0.0f, 0.0f, 0.5f}, p1{0.5f, 0.5f, 0.5f} {}
   Line(const Point3& p0, const Point3& p1):
	p0{p0}, p1{p1} {}
   Point3 p0;
   Point3 p1;
};
// class Point2{};
class Matrix3{
public:
   std::array<float, 9> buffer;
   float operator()(uint32_t line, uint32_t column) const {
      return buffer[line*3 + column];
   }
};
// identity factory
// diagonal factory
// rotation factory
// scale factory
// translation factory

class Matrix4{};

Vector3 operator+(const Vector3& a, const Vector3& b){
   return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
}
Vector3 operator-(const Vector3& a, const Vector3& b){
   return Vector3{a.x - b.x, a.y - b.y, a.z - b.z};
}
float m::abs(const Vector3& a){
   auto dotted = dot(a,a);
   return std::sqrt(dotted.x + dotted.y + dotted.z);
}
Vector3 m::dot(const Vector3& a, const Vector3& b){
   return Vector3{a.x * b.x, a.y * b.y, a.z * b.z};
}
Vector3 m::cross(const Vector3& a, const Vector3& b){
   // return can't remember lol ;
   return Vector3{};
}
Vector3 m::normal(const Vector3& v){
   auto abs = m::abs(v);
   return Vector3{v.x / abs, v.y / abs, v.z / abs};
}

Vector3 operator*(float a, const Vector3& b){
   return Vector3{a * b.x, a * b.y, a * b.z};
}
Vector3 operator*(const Vector3& b, float a){
   return a * b;
}
Vector3 operator/(const Vector3& b, float a){
   return b * 1.0f/a;
}

Vector3 operator*(const Matrix3& m, const Vector3 v){
   auto result = Vector3{};
   for(auto i = 0; i < 3; ++i){
      for(auto j = 0; j < 3; ++j){
         result[i] += m(i, j) * v(j);
      }
   }
   return result;
}

// Matrix3 operator?

std::ostream& operator<<(std::ostream& os, const Matrix3& m){
   char element_separator = ' ';
   auto line_counter = uchar{0};
   for(auto i = 0; i < 9; ++i){
      if (line_counter == 0 ){
         os << '\n';
      }
      os << m.buffer[i] << element_separator;
      ++line_counter;
      line_counter %= 3;
   }
   os << '\n';
   return os;
}
std::ostream& operator<<(std::ostream& os, const Vector3& v){
   char element_separator = ' ';
   os << "\n<" << v.x << element_separator
               << v.y << element_separator
               << v.z << element_separator << ">\n";
   return os;
}

#endif // __MATRIX_H__
