#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <math.h>
#include <stdio.h>


struct CSE_API csVector2f
{
public:
  float x;
  float y;

public:
  CS_FORCEINLINE csVector2f(const float *f)
    : x(f[0])
    , y(f[1])
  {

  }

  CS_FORCEINLINE csVector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  CS_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  CS_FORCEINLINE csVector2f &operator+=(const csVector2f &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator-=(const csVector2f &v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator*=(const csVector2f &v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator/=(const csVector2f &v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CS_FORCEINLINE static csVector2f &Add(const csVector2f &v0, const csVector2f &v1, csVector2f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  CS_FORCEINLINE static csVector2f &Add(const csVector2f &v0, float v1, csVector2f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  CS_FORCEINLINE static csVector2f &Sub(const csVector2f &v0, const csVector2f &v1, csVector2f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  CS_FORCEINLINE static csVector2f &Sub(const csVector2f &v0, float v1, csVector2f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  CS_FORCEINLINE static csVector2f &Mul(const csVector2f &v0, const csVector2f &v1, csVector2f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  CS_FORCEINLINE static csVector2f &Mul(const csVector2f &v0, float v1, csVector2f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  CS_FORCEINLINE static csVector2f &Div(const csVector2f &v0, const csVector2f &v1, csVector2f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  CS_FORCEINLINE static csVector2f &Div(const csVector2f &v0, float v1, csVector2f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  CS_FORCEINLINE float Dot() const
  {
    return x*x + y*y;
  }

  CS_FORCEINLINE float Dot(const csVector2f &v) const
  {
    return x*v.x + y*v.y;
  }

  CS_FORCEINLINE static float Dot(const csVector2f &v0, const csVector2f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y;
  }

  CS_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y);
  }

  CS_FORCEINLINE csVector2f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  CS_FORCEINLINE csVector2f &Normalized(csVector2f &r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};


