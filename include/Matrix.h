#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <array>

class Vector2{};
class Vector3{
public:
   float x{0.0f};
   float y{0.0f};
   float z{0.0f};
   Vector3 dot(const Vector3 b){
      return dot(*this, b);
   }
};
class Point2{};
class Point3{
public:
   float x;
   float y;
   float z;
};
class Matrix3{
public:
   std::array<float, 9> buffer;
   float operator()(uchar line, uchar column) const {
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
Vector3 dot(const Vector3& a, const Vector3& b){
   return Vector3{a.x * b.x, a.y * b.y, a.z * b.z};
}
Vector3 operator*(float a, const Vector& b){
   return Vector3{a * b.x, a * b.y, a * b.z};
}
Vector3 operator*(const Vector& b, float a){
   return a * b;
}
Vector3 operator/(const Vector& b, float a){
   return b * 1.0f/a;
}

Vector3 operator*(const Matrix3& m, const Vector3 v){
   return Vector3();
}

Matrix3

#endif // __MATRIX_H__