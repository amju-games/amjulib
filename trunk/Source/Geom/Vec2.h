#ifndef VEC2_H
#define VEC2_H

#include <math.h>

namespace Amju
{
static const float EPSILON = 0.0001f;

template <class T>
struct Vec2
{
  Vec2() : x(0), y(0) {}
  Vec2(T mx, T my) : x(mx), y(my) {}
  T x, y;

  bool operator==(const Vec2& v) const
  {
    return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON);
  }

  bool operator!=(const Vec2& v) const
  {
    return !(*this == v);
  }

  T SqLen() const
  {
    return x*x + y*y;
  }

  Vec2& operator+=(const Vec2& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vec2& operator-=(const Vec2& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vec2& operator*=(T f)
  {
    x *= f;
    y *= f;
    return *this;
  }

  void Normalise()
  {
    float n = 1.0f / sqrt(SqLen());
    x *= n;
    y *= n;
  }
};

template <class T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2)
{
  Vec2<T> v = v1;
  v += v2;
  return v;
}

template <class T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2)
{
  Vec2<T> v = v1;
  v -= v2;
  return v;
}

template <class T>
Vec2<T> operator*(const Vec2<T>& v1, T f)
{
  Vec2<T> v = v1;
  v *= f;
  return v;
}

template <class T>
float DotProduct(const Vec2<T>& v1, const Vec2<T>& v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

template <class T>
bool operator<(const Vec2<T>& v1, const Vec2<T>& v2)
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

  return false;
}

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

}

#endif
