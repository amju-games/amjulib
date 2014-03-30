#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <assert.h>
#include <math.h>

namespace Amju
{
template <class T>
struct Vec3
{
  T x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  Vec3(T mx, T my, T mz) : x(mx), y(my), z(mz) 
  {
    AssertValid();
  }

  void AssertValid() const
  {
#ifdef _DEBUG
    assert(x == x);
    assert(y == y);
    assert(z == z);
#endif
  }

  T SqLen() const
  {
    AssertValid();
    return x*x + y*y + z*z;
  }

  Vec3 operator-() const
  {
    AssertValid();
    return Vec3(-x, -y, -z);
  }

  Vec3& operator+=(const Vec3& v)
  {
    AssertValid();
    x += v.x;
    y += v.y;
    z += v.z;
    AssertValid();
    return *this;
  }

  Vec3& operator-=(const Vec3& v)
  {
    AssertValid();
    x -= v.x;
    y -= v.y;
    z -= v.z;
    AssertValid();
    return *this;
  }

  Vec3& operator*=(T f)
  {
    AssertValid();
    x *= f;
    y *= f;
    z *= f;
    AssertValid();
    return *this;
  }

  void Normalise()
  {
    AssertValid();
    float n = 1.0f / sqrt(SqLen());
    x *= n;
    y *= n;
    z *= n;
    AssertValid();
  }
};

template <class T>
Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2)
{
  v1.AssertValid();
  v2.AssertValid();
  Vec3<T> v = v1;
  v += v2;
  v.AssertValid();
  return v;
}

template <class T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2)
{
  v1.AssertValid();
  v2.AssertValid();
  Vec3<T> v = v1;
  v -= v2;
  v.AssertValid();
  return v;
}

template <class T>
Vec3<T> operator*(const Vec3<T>& v1, T f)
{
  v1.AssertValid();
  Vec3<T> v = v1;
  v *= f;
  v.AssertValid();
  return v;
}

template <class T>
Vec3<T> operator*(T f, const Vec3<T>& v1)
{
  v1.AssertValid();
  Vec3<T> v = v1;
  v *= f;
  v.AssertValid();
  return v;
}

template <class T>
Vec3<T> Normalise(const Vec3<T> v)
{
  v.AssertValid();
  Vec3<T> v1(v);
  v1.Normalise();
  v1.AssertValid();
  return v1;
}

template <class T>
Vec3<T> CrossProduct(const Vec3<T>& v1, const Vec3<T>& v2)
{
  v1.AssertValid();
  v2.AssertValid();
  return Vec3<T>(
    v1.y * v2.z - v2.y * v1.z,
    v2.x * v1.z - v1.x * v2.z,
    v1.x * v2.y - v2.x * v1.y);
}

template <class T>
float DotProduct(const Vec3<T>& v1, const Vec3<T>& v2)
{
  v1.AssertValid();
  v2.AssertValid();
  return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
}

template <class T>
bool operator<(const Vec3<T>& v1, const Vec3<T>& v2)
{
  v1.AssertValid();
  v2.AssertValid();
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
