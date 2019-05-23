#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <math.h>
#include <stdio.h>


namespace cs
{

struct CSE_API Vector2f
{
public:
  float x;
  float y;

public:
  CS_FORCEINLINE Vector2f(const float* f)
    : x(f[0])
    , y(f[1])
  {

  }

  CS_FORCEINLINE Vector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  CS_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  CS_FORCEINLINE cs::Vector2f& operator+=(const cs::Vector2f& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator-=(const cs::Vector2f& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator*=(const cs::Vector2f& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator/=(const cs::Vector2f& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CS_FORCEINLINE static cs::Vector2f& Add(const cs::Vector2f& v0, const cs::Vector2f& v1, cs::Vector2f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  CS_FORCEINLINE static cs::Vector2f& Add(const cs::Vector2f& v0, float v1, cs::Vector2f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  CS_FORCEINLINE static cs::Vector2f& Sub(const cs::Vector2f& v0, const cs::Vector2f& v1, cs::Vector2f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  CS_FORCEINLINE static cs::Vector2f& Sub(const cs::Vector2f& v0, float v1, cs::Vector2f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  CS_FORCEINLINE static cs::Vector2f& Mul(const cs::Vector2f& v0, const cs::Vector2f& v1, cs::Vector2f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  CS_FORCEINLINE static cs::Vector2f& Mul(const cs::Vector2f& v0, float v1, cs::Vector2f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  CS_FORCEINLINE static cs::Vector2f& Div(const cs::Vector2f& v0, const cs::Vector2f& v1, cs::Vector2f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  CS_FORCEINLINE static cs::Vector2f& Div(const cs::Vector2f& v0, float v1, cs::Vector2f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  CS_FORCEINLINE float Dot(const cs::Vector2f& v) const
  {
    return x * v.x + y * v.y;
  }

  CS_FORCEINLINE static float Dot(const cs::Vector2f& v0, const cs::Vector2f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }

  CS_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y);
  }

  CS_FORCEINLINE cs::Vector2f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  CS_FORCEINLINE cs::Vector2f& Normalized(cs::Vector2f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};



}

