#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <math.h>

namespace Amju
{
template <class T>
struct Vec3
{
  T x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  Vec3(T mx, T my, T mz) : x(mx), y(my), z(mz) {}

  T SqLen() const
  {
    return x*x + y*y + z*z;
  }

  Vec3 operator-() const
  {
    return Vec3(-x, -y, -z);
  }

  Vec3& operator+=(const Vec3& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vec3& operator-=(const Vec3& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  Vec3& operator*=(T f)
  {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  void Normalise()
  {
    float n = 1.0f / sqrt(SqLen());
    x *= n;
    y *= n;
    z *= n;
  }
};

template <class T>
Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2)
{
  Vec3<T> v = v1;
  v += v2;
  return v;
}

template <class T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2)
{
  Vec3<T> v = v1;
  v -= v2;
  return v;
}

template <class T>
Vec3<T> operator*(const Vec3<T>& v1, T f)
{
  Vec3<T> v = v1;
  v *= f;
  return v;
}

template <class T>
Vec3<T> operator*(T f, const Vec3<T>& v1)
{
  Vec3<T> v = v1;
  v *= f;
  return v;
}

template <class T>
Vec3<T> Normalise(const Vec3<T> v)
{
  Vec3<T> v1(v);
  v1.Normalise();
  return v1;
}

template <class T>
Vec3<T> CrossProduct(const Vec3<T>& v1, const Vec3<T>& v2)
{
  return Vec3<T>(
    v1.y * v2.z - v2.y * v1.z,
    v2.x * v1.z - v1.x * v2.z,
    v1.x * v2.y - v2.x * v1.y);
}

template <class T>
float DotProduct(const Vec3<T>& v1, const Vec3<T>& v2)
{
  return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
}

template <class T>
bool operator<(const Vec3<T>& v1, const Vec3<T>& v2)
{
  if (v1.x < v2.x) 
  {
    return true;
  }
  else if (v1.x > v2.x)
  {
    return false;
  }

  if (v1.y < v2.y) 
  {
    return true;
  }
  else if (v1.y > v2.y)
  {
    return false;
  }

  if (v1.z < v2.z) 
  {
    return true;
  }

  return false;
}

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
}

#endif
